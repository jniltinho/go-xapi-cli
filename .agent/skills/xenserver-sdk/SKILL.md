# XenServer Go SDK — Developer Reference

Official Go SDK for the XenServer/XCP-ng API. Manages pools, hosts, VMs, disks, networks and other resources via JSON-RPC 2.0 over HTTP/HTTPS.

---

## Package Structure

```
src/
├── go.mod               # Module: go/xenapi, requires Go 1.22.2
├── jsonrpc_client.go    # Low-level HTTP/JSON-RPC client
├── session.go           # Session management and authentication
├── auth.go              # External authentication (LDAP/AD)
├── api_versions.go      # API versions and auto-detection
├── api_errors.go        # API error constants (Error*)
├── api_messages.go      # API message constants
├── enums.go             # All enum types used by the API
├── convert.go           # Type serialization/deserialization
├── convert_test.go      # Conversion tests
├── export_test.go       # Internal test exports
│
├── vm.go                # Virtual Machine
├── host.go              # Physical host
├── pool.go              # Host pool
├── vdi.go               # Virtual Disk Image
├── sr.go                # Storage Repository
├── vbd.go               # Virtual Block Device
├── vif.go               # Virtual Network Interface
├── pif.go               # Physical Network Interface
├── network.go           # Virtual networks
├── task.go              # Asynchronous tasks
├── event.go             # Event system
├── session.go           # User sessions
└── ...                  # Other resources (bond, vlan, tunnel, etc.)
```

---

## Core Concepts

### Resource Pattern

Every API resource follows the same Go pattern:

```go
// 1. Record — struct with all object fields
type VMRecord struct {
    UUID        string
    NameLabel   string
    PowerState  VMPowerState
    // ...
}

// 2. Ref — opaque reference (string) to the server object
type VMRef string

// 3. Singleton — empty struct holding the API methods
type vm struct{}
var VM vm   // global variable to call methods
```

**Usage:**
```go
refs, err   := VM.GetAll(session)
record, err := VM.GetRecord(session, vmRef)
name, err   := VM.GetNameLabel(session, vmRef)
```

---

## Connection and Authentication

### Creating a Session

```go
import "go/xenapi"

opts := &xenapi.ClientOpts{
    URL:     "https://192.168.1.100",
    Timeout: 30, // seconds (0 = no timeout)
    Headers: map[string]string{
        "X-Custom-Header": "value",
    },
}

// Creates the session object (does not authenticate yet)
session := xenapi.NewSession(opts)

// Authenticate — populates session.ref, session.APIVersion, session.XAPIVersion
_, err := session.LoginWithPassword("root", "password", "1.0", "my-app")
if err != nil {
    log.Fatal(err)
}
defer session.Logout()
```

### TLS / HTTPS

```go
opts := &xenapi.ClientOpts{
    URL: "https://192.168.1.100",
    SecureOpts: &xenapi.SecureOpts{
        ServerCert: "/path/to/ca.pem",     // CA cert (disables InsecureSkipVerify)
        ClientCert: "/path/to/client.crt", // Mutual TLS (optional)
        ClientKey:  "/path/to/client.key",
    },
}
```

> **Warning:** Without `ServerCert`, TLS verification is disabled (`InsecureSkipVerify: true`). Always provide the CA certificate in production.

### LoginWithPassword Variants

| Method | Parameters | Min version |
|--------|-----------|-------------|
| `LoginWithPassword2` | uname, pwd | rio (1.1) |
| `LoginWithPassword3` | uname, pwd, version | miami (1.2) |
| `LoginWithPassword`  | uname, pwd, version, originator | clearwater (2.0) |

After a successful login, `session.APIVersion` and `session.XAPIVersion` are auto-detected from the pool master.

### Emergency / Slave Login

```go
// For hosts in emergency mode (no coordinator)
_, err := session.SlaveLocalLoginWithPassword("root", "password")
```

---

## Session Fields

```go
type Session struct {
    APIVersion  APIVersion // server API version
    XAPIVersion string     // XAPI version string (e.g. "24.10.0")
    // ref and client are private
}
```

---

## API Versions

```go
// Available constants:
// APIVersion1_1  (XenServer 4.0 / rio)
// APIVersion1_2  (XenServer 4.1 / miami)
// ...
// APIVersion2_15 (Citrix Hypervisor 8.2 / stockholm)
// APIVersion2_20 (XenServer 8 Preview / nile-preview)
// APIVersion2_21 (XenServer 8 / nile)
// APIVersionLatest = 28
// APIVersionUnknown = 99

if session.APIVersion >= xenapi.APIVersion2_15 {
    // feature available only on 8.2+
}

ver := xenapi.GetAPIVersion(2, 15) // returns APIVersion2_15
```

---

## Asynchronous Operations (Tasks)

Many methods have an `Async*` variant that returns a `TaskRef` instead of the result:

```go
// Synchronous
err := VM.Start(session, vmRef, false, false)

// Asynchronous — returns TaskRef immediately
taskRef, err := VM.AsyncStart(session, vmRef, false, false)
if err != nil {
    log.Fatal(err)
}

// Poll until done
for {
    record, _ := Task.GetRecord(session, taskRef)
    switch record.Status {
    case xenapi.TaskStatusTypeSuccess:
        fmt.Println("Done!")
        return
    case xenapi.TaskStatusTypeFailure:
        fmt.Println("Failed:", record.ErrorInfo)
        return
    }
    time.Sleep(1 * time.Second)
}
```

**TaskRecord key fields:**

| Field | Type | Description |
|-------|------|-------------|
| `Status` | `TaskStatusType` | `pending`, `success`, `failure`, `cancelling`, `cancelled` |
| `Progress` | `float64` | 0.0–1.0 estimated completion |
| `Result` | `string` | Result object reference (on success) |
| `ErrorInfo` | `[]string` | Error strings (on failure) |
| `Subtasks` | `[]TaskRef` | Nested sub-tasks |

---

## Core Resources

### VM (Virtual Machine)

**VMRecord key fields:**

| Field | Type | Description |
|-------|------|-------------|
| `UUID` | `string` | Unique identifier |
| `NameLabel` | `string` | Display name |
| `PowerState` | `VMPowerState` | `Halted`, `Running`, `Suspended`, `Paused` |
| `IsATemplate` | `bool` | true = template (cannot start) |
| `IsControlDomain` | `bool` | true = dom0 (skip in listings) |
| `ResidentOn` | `HostRef` | Host where VM is running |
| `MemoryStaticMax` | `int` | Max memory in bytes |
| `VCPUsMax` | `int` | Max vCPUs |
| `VIFs` | `[]VIFRef` | Virtual network interfaces |
| `VBDs` | `[]VBDRef` | Virtual block devices |

**Common operations:**
```go
// List all VMs (excluding templates and dom0)
allRefs, _ := VM.GetAll(session)
for _, ref := range allRefs {
    rec, _ := VM.GetRecord(session, ref)
    if rec.IsATemplate || rec.IsControlDomain {
        continue
    }
    fmt.Println(rec.NameLabel, rec.PowerState)
}

// Find by name
refs, _ := VM.GetByNameLabel(session, "my-vm")

// Lifecycle
VM.Start(session, ref, false, false) // startPaused=false, force=false
VM.Shutdown(session, ref)
VM.Reboot(session, ref)
VM.Suspend(session, ref)
VM.Resume(session, ref, false, false)

// Snapshot
snapRef, _ := VM.Snapshot(session, ref, "snap-name")

// Clone
newRef, _ := VM.Clone(session, ref, "new-name")

// Migrate to another host
VM.PoolMigrate(session, ref, hostRef, map[string]string{})
```

---

### Host

**HostRecord key fields:**

| Field | Type | Description |
|-------|------|-------------|
| `NameLabel` | `string` | Host name |
| `Enabled` | `bool` | Host accepting new VMs |
| `SoftwareVersion` | `map[string]string` | Software versions (includes `"xapi"`) |
| `ResidentVMs` | `[]VMRef` | VMs currently running here |
| `PIFs` | `[]PIFRef` | Physical network interfaces |
| `MemoryOverhead` | `int` | Hypervisor overhead (bytes) |

```go
hosts, _ := Host.GetAll(session)
for _, hRef := range hosts {
    rec, _ := Host.GetRecord(session, hRef)
    fmt.Println(rec.NameLabel, "enabled:", rec.Enabled)
}

Host.Disable(session, hostRef)  // maintenance mode
Host.Enable(session, hostRef)
Host.Evacuate(session, hostRef) // move all VMs away
```

---

### Pool

**PoolRecord key fields:**

| Field | Type | Description |
|-------|------|-------------|
| `Master` | `HostRef` | Pool coordinator host |
| `DefaultSR` | `SRRef` | Default SR for new VDIs |
| `HaEnabled` | `bool` | High availability active |

```go
pools, _ := Pool.GetAll(session)
rec, _ := Pool.GetRecord(session, pools[0])
fmt.Println("Master:", rec.Master, "HA:", rec.HaEnabled)
```

---

### SR — Storage Repository

```go
srs, _ := SR.GetAll(session)
for _, srRef := range srs {
    rec, _ := SR.GetRecord(session, srRef)
    freeMB := (rec.PhysicalSize - rec.PhysicalUtilisation) / 1024 / 1024
    fmt.Printf("SR: %s | Type: %s | Free: %d MB\n",
        rec.NameLabel, rec.Type, freeMB)
}
```

---

### VDI — Virtual Disk Image

```go
// Create
vdiRef, _ := VDI.Create(session, xenapi.VDIRecord{
    NameLabel:   "my-disk",
    SR:          srRef,
    VirtualSize: 10 * 1024 * 1024 * 1024, // 10 GB
    Type:        xenapi.VdiTypeUser,
})

// Copy to another SR
newVDI, _ := VDI.Copy(session, vdiRef, srRef)

// Snapshot
snapVDI, _ := VDI.Snapshot(session, vdiRef, map[string]string{})
```

---

### VBD — Virtual Block Device (disk attachment)

```go
vbdRef, _ := VBD.Create(session, xenapi.VBDRecord{
    VM:       vmRef,
    VDI:      vdiRef,
    Device:   "xvdb",
    Mode:     xenapi.VbdModeRW,
    Type:     xenapi.VbdTypeDisk,
    Bootable: false,
})
VBD.Plug(session, vbdRef)
VBD.Unplug(session, vbdRef)
```

---

### VIF — Virtual Network Interface

```go
vifRef, _ := VIF.Create(session, xenapi.VIFRecord{
    VM:      vmRef,
    Network: networkRef,
    Device:  "0",
    MTU:     1500,
})
VIF.Plug(session, vifRef)
```

---

### Events

```go
Event.Register(session, []string{"vm", "host"})
defer Event.Unregister(session, []string{"vm", "host"})

token := ""
for {
    batch, err := Event.From(session, []string{"vm"}, token, 30.0)
    if err != nil {
        break
    }
    token = batch.Token
    for _, ev := range batch.Events {
        fmt.Println(ev.Operation, ev.Class, ev.ObjUUID)
    }
}
```

**EventRecord fields:**

| Field | Type | Description |
|-------|------|-------------|
| `Operation` | `EventOperation` | `add`, `del`, `mod` |
| `Class` | `string` | Object class (e.g. `"vm"`) |
| `Ref` | `string` | Object reference |
| `ObjUUID` | `string` | Object UUID |
| `Snapshot` | `RecordInterface` | Object state at event time |

---

## Error Handling

### Error Constants

```go
xenapi.ErrorSessionAuthenticationFailed // wrong credentials
xenapi.ErrorVMBadPowerState             // wrong state for the operation
xenapi.ErrorSrFull                      // SR out of space
xenapi.ErrorHostNotEnoughFreeMemory     // host out of memory
xenapi.ErrorRbacPermissionDenied        // RBAC access denied
xenapi.ErrorHaIsEnabled                 // operation blocked by active HA
```

Error format returned by API calls:
```
API error: code <N>, message <MSG>[, data <DATA>]
```

---

## Key Enums

### VMPowerState
```go
VMPowerStateHalted    // powered off
VMPowerStateRunning   // running
VMPowerStateSuspended // suspended to disk
VMPowerStatePaused    // paused (CPU halted)
```

### VdiType
```go
VdiTypeSystem    // OS system disk
VdiTypeUser      // data disk
VdiTypeEphemeral // ephemeral disk
VdiTypeSuspend   // suspend image
VdiTypeCrashdump // crash dump
```

### TaskStatusType
```go
TaskStatusTypePending    // in progress
TaskStatusTypeSuccess    // completed successfully
TaskStatusTypeFailure    // failed
TaskStatusTypeCancelling // being cancelled
TaskStatusTypeCancelled  // cancelled
```

---

## Numbered Methods (API Versions)

Many methods have versioned variants with numeric suffixes (`Method`, `Method2`, `Method3`...), each corresponding to an API version with a different parameter set.

**Rule:** Always use the **un-suffixed** method (or highest number) for the most complete/recent version. Numbered variants exist for backward compatibility with older servers.

```go
// Prefer (most complete, clearwater+):
session.LoginWithPassword(uname, pwd, version, originator)

// Fallback for old servers (rio+):
session.LoginWithPassword2(uname, pwd)
```

---

## All Available Resources

| File | Variable | Description |
|------|----------|-------------|
| `vm.go` | `VM` | Virtual machines |
| `host.go` | `Host` | Physical hosts |
| `pool.go` | `Pool` | Host pools |
| `vdi.go` | `VDI` | Virtual disk images |
| `sr.go` | `SR` | Storage repositories |
| `vbd.go` | `VBD` | Virtual block devices |
| `vbd_metrics.go` | `VBDMetrics` | Block device metrics |
| `vif.go` | `VIF` | Virtual network interfaces |
| `vif_metrics.go` | `VIFMetrics` | VIF metrics |
| `pif.go` | `PIF` | Physical network interfaces |
| `pif_metrics.go` | `PIFMetrics` | PIF metrics |
| `network.go` | `Network` | Virtual networks |
| `network_sriov.go` | `NetworkSriov` | SR-IOV networks |
| `bond.go` | `Bond` | NIC bonding |
| `vlan.go` | `VLAN` | VLANs on PIFs |
| `tunnel.go` | `Tunnel` | Network tunnels |
| `task.go` | `Task` | Async tasks |
| `event.go` | `Event` | Event system |
| `session.go` | `Session` | User sessions |
| `auth.go` | `Auth` | External authentication |
| `host_cpu.go` | `HostCPU` | Physical CPU details |
| `host_metrics.go` | `HostMetrics` | Host free/total memory |
| `host_crashdump.go` | `HostCrashdump` | Host crash dumps |
| `host_patch.go` | `HostPatch` | Applied host patches |
| `vm_metrics.go` | `VMMetrics` | VM uptime, vCPU usage |
| `vm_guest_metrics.go` | `VMGuestMetrics` | Guest IP, OS, PV drivers |
| `vm_appliance.go` | `VMAppliance` | VM appliances |
| `pbd.go` | `PBD` | Physical block device (host-SR link) |
| `sm.go` | `SM` | Storage manager plugins |
| `sr_stat.go` | `SRStat` | SR statistics |
| `console.go` | `Console` | VM console |
| `crashdump.go` | `Crashdump` | VM crash dumps |
| `message.go` | `Message` | System messages |
| `secret.go` | `Secret` | Secure string storage |
| `role.go` | `Role` | RBAC roles |
| `subject.go` | `Subject` | External auth subjects |
| `certificate.go` | `Certificate` | Pool TLS certificates |
| `repository.go` | `Repository` | Update repositories |
| `cluster.go` | `Cluster` | Clustering |
| `cluster_host.go` | `ClusterHost` | Cluster host membership |
| `observer.go` | `Observer` | Observability / tracing |
| `pvs_site.go` | `PVSSite` | PVS site |
| `pvs_server.go` | `PVSServer` | PVS server |
| `pvs_proxy.go` | `PVSProxy` | PVS proxy |
| `pvs_cache_storage.go` | `PVSCacheStorage` | PVS cache storage |
| `gpu_group.go` | `GPUGroup` | GPU groups |
| `pgpu.go` | `PGPU` | Physical GPUs |
| `vgpu.go` | `VGPU` | Virtual GPUs |
| `vgpu_type.go` | `VGPUType` | VGPU types |
| `dr_task.go` | `DRTask` | Disaster recovery tasks |
| `vmpp.go` | `VMPP` | VM protection policies |
| `vmss.go` | `VMSS` | VM snapshot schedules |
| `vtpm.go` | `VTPM` | Virtual TPM |
| `vusb.go` | `VUSB` | Virtual USB |
| `pusb.go` | `PUSB` | Physical USB |
| `usb_group.go` | `USBGroup` | USB groups |
| `blob.go` | `Blob` | Binary data storage |
| `feature.go` | `Feature` | Host feature flags |
| `data_source.go` | `DataSource` | RRD data sources |
| `lvhd.go` | `LVHD` | LVHD storage driver |
| `probe_result.go` | `ProbeResult` | SR probe results |
| `sdn_controller.go` | `SDNController` | SDN controllers |
| `driver_variant.go` | `DriverVariant` | Host driver variants |

---

## Important Notes

1. **Numbered methods**: Always use the un-suffixed variant unless you need backward compatibility with older XenServer versions.
2. **`Async*` for long operations**: `VM.AsyncStart`, `Host.AsyncEvacuate`, etc. return `TaskRef` — poll with `Task.GetRecord`.
3. **Float serialization**: `convert.go` handles `NaN`, `+Inf`, `-Inf` which standard JSON does not support.
4. **Refs are opaque strings**: `VMRef`, `HostRef`, etc. are just `type XxxRef string` — pass them directly to methods.
5. **Global singletons are stateless**: `VM.GetAll(...)`, `Host.GetRecord(...)` — safe to call from multiple goroutines.
6. **Session is NOT goroutine-safe**: Create separate sessions for parallel goroutines.
7. **Always filter dom0**: Skip VMs where `record.IsControlDomain == true` in most application listings.
