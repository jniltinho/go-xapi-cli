package main

import (
	"fmt"
	"log"
	"sort"
	"strings"
	"time"

	"xenapi"

	"github.com/spf13/cobra"
)

func newSnapshotCmd() *cobra.Command {
	var vmName string
	var retain int

	cmd := &cobra.Command{
		Use:   "snap",
		Short: "Create a snapshot of a VM with optional retention",
		Run: func(cmd *cobra.Command, args []string) {
			session := newSession()
			defer session.Logout()
			createSnapshot(session, vmName, retain)
		},
	}

	cmd.Flags().StringVar(&vmName, "vm", "", "Name of the target VM (required)")
	cmd.Flags().IntVar(&retain, "retain", 7, "Number of snapshots to keep (0 = unlimited)")
	cmd.MarkFlagRequired("vm")

	return cmd
}

func createSnapshot(session *xenapi.Session, vmName string, retain int) {
	vmsRef, err := xenapi.VM.GetByNameLabel(session, vmName)
	if err != nil {
		log.Fatalf("Failed to fetch VM: %v", err)
	}
	if len(vmsRef) == 0 {
		log.Fatalf("VM '%s' not found", vmName)
	}

	targetVM := vmsRef[0]
	prefix := fmt.Sprintf("auto-snap-%s-", vmName)
	snapshotName := fmt.Sprintf("%s%d", prefix, time.Now().Unix())

	fmt.Printf("Creating snapshot '%s'...\n", snapshotName)

	snapshotRef, err := xenapi.VM.Snapshot(session, targetVM, snapshotName, nil)
	if err != nil {
		log.Fatalf("Failed to create snapshot: %v", err)
	}

	record, err := xenapi.VM.GetRecord(session, snapshotRef)
	if err != nil {
		log.Fatalf("Failed to retrieve snapshot record: %v", err)
	}

	fmt.Printf("Snapshot created successfully. UUID: %s\n", record.UUID)

	if retain > 0 {
		applyRetention(session, targetVM, prefix, retain)
	}
}

func applyRetention(session *xenapi.Session, vmRef xenapi.VMRef, prefix string, retain int) {
	snapRefs, err := xenapi.VM.GetSnapshots(session, vmRef)
	if err != nil {
		log.Printf("Warning: failed to list snapshots: %v", err)
		return
	}

	type snap struct {
		ref  xenapi.VMRef
		time time.Time
	}

	var snaps []snap
	for _, ref := range snapRefs {
		rec, err := xenapi.VM.GetRecord(session, ref)
		if err != nil {
			continue
		}
		if strings.HasPrefix(rec.NameLabel, prefix) {
			snaps = append(snaps, snap{ref: ref, time: rec.SnapshotTime})
		}
	}

	if len(snaps) <= retain {
		return
	}

	sort.Slice(snaps, func(i, j int) bool {
		return snaps[i].time.Before(snaps[j].time)
	})

	toDelete := snaps[:len(snaps)-retain]
	for _, s := range toDelete {
		if err := destroySnapshot(session, s.ref); err != nil {
			log.Printf("Warning: failed to destroy snapshot %s: %v", s.ref, err)
		} else {
			fmt.Printf("Deleted old snapshot (retention=%d): %s\n", retain, s.time.Format(time.DateTime))
		}
	}
}

func destroySnapshot(session *xenapi.Session, snapRef xenapi.VMRef) error {
	vbds, err := xenapi.VM.GetVBDs(session, snapRef)
	if err != nil {
		return fmt.Errorf("get VBDs: %w", err)
	}

	for _, vbdRef := range vbds {
		vdiRef, err := xenapi.VBD.GetVDI(session, vbdRef)
		if err != nil || vdiRef == "OpaqueRef:NULL" {
			continue
		}
		if err := xenapi.VDI.Destroy(session, vdiRef); err != nil {
			log.Printf("Warning: failed to destroy VDI %s: %v", vdiRef, err)
		}
	}

	return xenapi.VM.Destroy(session, snapRef)
}
