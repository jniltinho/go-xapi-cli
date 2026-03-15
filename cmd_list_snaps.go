package main

import (
	"log"
	"os"
	"sort"
	"time"

	"xenapi"

	"github.com/jedib0t/go-pretty/v6/table"
	"github.com/spf13/cobra"
)

func listSnapshotsCmd() *cobra.Command {
	var vmName string

	cmd := &cobra.Command{
		Use:   "list-snap",
		Short: "List all snapshots of a VM",
		Run: func(cmd *cobra.Command, args []string) {
			session := newSession()
			defer session.Logout()
			listSnapshots(session, vmName)
		},
	}

	cmd.Flags().StringVar(&vmName, "vm", "", "Name of the target VM (required)")
	cmd.MarkFlagRequired("vm")

	return cmd
}

func listSnapshots(session *xenapi.Session, vmName string) {
	vmsRef, err := xenapi.VM.GetByNameLabel(session, vmName)
	if err != nil {
		log.Fatalf("Failed to fetch VM: %v", err)
	}
	if len(vmsRef) == 0 {
		log.Fatalf("VM '%s' not found", vmName)
	}

	snapRefs, err := xenapi.VM.GetSnapshots(session, vmsRef[0])
	if err != nil {
		log.Fatalf("Failed to list snapshots: %v", err)
	}

	type snap struct {
		name string
		uuid string
		time time.Time
	}

	var snaps []snap
	for _, ref := range snapRefs {
		rec, err := xenapi.VM.GetRecord(session, ref)
		if err != nil {
			continue
		}
		snaps = append(snaps, snap{name: rec.NameLabel, uuid: rec.UUID, time: rec.SnapshotTime.In(time.FixedZone("UTC-3", -3*60*60))})
	}

	sort.Slice(snaps, func(i, j int) bool {
		return snaps[i].time.Before(snaps[j].time)
	})

	t := table.NewWriter()
	t.SetOutputMirror(os.Stdout)
	t.AppendHeader(table.Row{"#", "Name", "UUID", "Created At"})

	for i, s := range snaps {
		t.AppendRow([]any{i + 1, s.name, s.uuid, s.time.Format(time.DateTime)})
		t.AppendSeparator()
	}

	t.AppendFooter(table.Row{"", "", "Total", len(snaps)})
	t.Render()
}

func listAllSnapshotsCmd() *cobra.Command {
	return &cobra.Command{
		Use:   "list-all-snaps",
		Short: "List all snapshots in the host",
		Run: func(cmd *cobra.Command, args []string) {
			session := newSession()
			defer session.Logout()
			listAllSnapshots(session)
		},
	}
}

func listAllSnapshots(session *xenapi.Session) {
	vms, err := xenapi.VM.GetAll(session)
	if err != nil {
		log.Fatalf("Failed to fetch VMs: %v", err)
	}

	type snap struct {
		vmName string
		name   string
		uuid   string
		time   time.Time
	}

	var snaps []snap
	for _, vmRef := range vms {
		rec, err := xenapi.VM.GetRecord(session, vmRef)
		if err != nil || !rec.IsASnapshot {
			continue
		}
		snaps = append(snaps, snap{
			vmName: string(rec.SnapshotOf),
			name:   rec.NameLabel,
			uuid:   rec.UUID,
			time:   rec.SnapshotTime.In(time.FixedZone("UTC-3", -3*60*60)),
		})
	}

	sort.Slice(snaps, func(i, j int) bool {
		return snaps[i].time.Before(snaps[j].time)
	})

	t := table.NewWriter()
	t.SetOutputMirror(os.Stdout)
	t.AppendHeader(table.Row{"#", "Snapshot Name", "UUID", "Created At"})

	for i, s := range snaps {
		t.AppendRow([]any{i + 1, s.name, s.uuid, s.time.Format(time.DateTime)})
		t.AppendSeparator()
	}

	t.AppendFooter(table.Row{"", "", "Total", len(snaps)})
	t.Render()
}
