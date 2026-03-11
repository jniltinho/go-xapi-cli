package main

import (
	"fmt"
	"log"
	"os"

	"xenapi"

	"github.com/jedib0t/go-pretty/v6/table"
	"github.com/spf13/cobra"
)

func newListCmd() *cobra.Command {
	return &cobra.Command{
		Use:   "list",
		Short: "List all VMs",
		Run: func(cmd *cobra.Command, args []string) {
			session := newSession()
			defer session.Logout()
			listVMs(session)
		},
	}
}

func listVMs(session *xenapi.Session) {
	vms, err := xenapi.VM.GetAll(session)
	if err != nil {
		log.Fatal(err)
	}

	count := 0

	t := table.NewWriter()
	t.SetOutputMirror(os.Stdout)
	t.AppendHeader(table.Row{"Name", "UUID", "Power State", "RAM (GB)"})

	for _, vm := range vms {
		record, err := xenapi.VM.GetRecord(session, vm)
		if err != nil {
			continue
		}

		if record.IsATemplate || record.IsControlDomain {
			continue
		}

		ram := float64(record.MemoryStaticMax) / 1024 / 1024 / 1024

		t.AppendRow([]any{record.NameLabel, record.UUID, record.PowerState, fmt.Sprintf("%.2f GB", ram)})
		t.AppendSeparator()

		count++
	}

	t.AppendFooter(table.Row{"", "", "Total VMs", count})
	t.Render()
}
