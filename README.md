# xapi-cli

A command-line tool for managing XCP-ng / XenServer hosts via the XAPI JSON-RPC interface, written in Go.

## Requirements

- Go 1.26+
- [UPX](https://upx.github.io/) (used by `make build` and `make build-prod` to compress the binary)

## Build

```bash
# Development build
make build

# Production build (static binary, stripped symbols)
make build-prod
```

The binary is output to `./xapi-cli`.

## Release

Releases are automated via GitHub Actions. Basta criar e fazer push de uma tag `v*`:

```bash
git tag v0.2.0
git push origin v0.2.0
```

O workflow irá compilar o binário, empacotá-lo em `xapi-cli_{VERSION}_linux_amd64.tar.gz` e criar a release no GitHub automaticamente.

## Configuration

Credentials can be provided via environment variables or CLI flags. **Flags take precedence over environment variables.**

| Environment Variable | Flag     | Default       | Description             |
|----------------------|----------|---------------|-------------------------|
| `XEN_HOST`           | `--host` | `192.168.0.10`| XenServer host IP or hostname |
| `XEN_USER`           | `--user` | `root`        | XenServer username      |
| `XEN_PASS`           | `--pass` | *(empty)*     | XenServer password      |

```bash
export XEN_HOST=192.168.1.10
export XEN_USER=root
export XEN_PASS=secret
```

## Commands

### `list` — List all VMs

```bash
xapi-cli list
xapi-cli --host 192.168.1.10 --user root --pass secret list
```

### `snap` — Create a snapshot

Creates a snapshot named `auto-snap-<vmname>-<unix-timestamp>` and applies retention.

```bash
# Create a snapshot, keep the 7 most recent (default)
xapi-cli snap --vm "MyVM"

# Keep only the 3 most recent snapshots
xapi-cli snap --vm "MyVM" --retain 3

# Unlimited retention (no automatic deletion)
xapi-cli snap --vm "MyVM" --retain 0
```

| Flag       | Default | Description                              |
|------------|---------|------------------------------------------|
| `--vm`     | *(required)* | Name of the target VM             |
| `--retain` | `7`     | Number of snapshots to keep (`0` = unlimited) |

### `list-snap` — List snapshots of a VM

Lists all snapshots of a VM sorted by creation date (displayed in UTC-3).

```bash
xapi-cli list-snap --vm "MyVM"
```

Output example:

```
+---+---------------------------+--------------------------------------+---------------------+
| # | Name                      | UUID                                 | Created At          |
+---+---------------------------+--------------------------------------+---------------------+
| 1 | auto-snap-MyVM-1740000000 | xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx | 2026-03-01 10:00:00 |
| 2 | auto-snap-MyVM-1741000000 | yyyyyyyy-yyyy-yyyy-yyyy-yyyyyyyyyyyy | 2026-03-10 08:30:00 |
+---+---------------------------+--------------------------------------+---------------------+
|   |                           | Total                                |                   2 |
+---+---------------------------+--------------------------------------+---------------------+
```

### `list-all-snaps` — List all snapshots in the host

Lists all snapshots across all VMs, sorted by creation date (displayed in UTC-3).

```bash
xapi-cli list-all-snaps
```

## Project Structure

```
main.go                                    # Root command, global flags, entrypoint
session.go                                 # XenServer session helper
cmd_list.go                                # list command
cmd_snapshot.go                            # snap command (create, retention, destroy)
cmd_list_snaps.go                          # list-snap and list-all-snaps commands
Makefile                                   # build, build-prod, deps, clean targets
DOCS/XenServer-SDK/XenServerGo/src/        # xenapi SDK (package xenapi)
.github/workflows/release.yml             # GitHub Actions release workflow
```
