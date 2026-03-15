package main

import (
	"fmt"
	"os"

	"github.com/spf13/cobra"
)

var (
	host string
	user string
	pass string
)

var (
	Version   = "v0.1.0"
	BuildDate = "unknown"
)

func envOr(key, fallback string) string {
	if v := os.Getenv(key); v != "" {
		return v
	}
	return fallback
}

func main() {
	root := &cobra.Command{
		Use:   "xapi-cli",
		Short: "XCP-ng / XenServer management CLI",
	}

	root.PersistentFlags().StringVar(&host, "host", envOr("XEN_HOST", "192.168.0.10"), "XenServer host IP or hostname (env: XEN_HOST)")
	root.PersistentFlags().StringVar(&user, "user", envOr("XEN_USER", "root"), "XenServer username (env: XEN_USER)")
	root.PersistentFlags().StringVar(&pass, "pass", envOr("XEN_PASS", ""), "XenServer password (env: XEN_PASS)")

	root.Version = Version
	root.SetVersionTemplate(fmt.Sprintf("XAPI-CLI version %s (Build Date: %s)\n", Version, BuildDate))
	root.AddCommand(newListCmd(), newSnapshotCmd(), listSnapshotsCmd(), listAllSnapshotsCmd())

	if err := root.Execute(); err != nil {
		os.Exit(1)
	}
}
