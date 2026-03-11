/*
 * Copyright (c) Cloud Software Group, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1) Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2) Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */


using System;
using System.Collections;
using System.Collections.Generic;
using System.Management.Automation;
using XenAPI;

namespace Citrix.XenServer.Commands
{
    [Cmdlet(VerbsCommon.New, "XenVM", DefaultParameterSetName = "Hashtable", SupportsShouldProcess = true)]
    [OutputType(typeof(XenAPI.VM))]
    [OutputType(typeof(XenAPI.Task))]
    [OutputType(typeof(void))]
    public class NewXenVMCommand : XenServerCmdlet
    {
        #region Cmdlet Parameters

        [Parameter]
        public SwitchParameter PassThru { get; set; }

        [Parameter(ParameterSetName = "Hashtable", Mandatory = true)]
        public Hashtable HashTable { get; set; }

        [Parameter(ParameterSetName = "Record", Mandatory = true)]
        public XenAPI.VM Record { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string NameLabel { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string NameDescription { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public vm_power_state PowerState { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long UserVersion { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool IsATemplate { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.VDI> SuspendVDI { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.Host> Affinity { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long MemoryTarget { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long MemoryStaticMax { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long MemoryDynamicMax { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long MemoryDynamicMin { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long MemoryStaticMin { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable VCPUsParams { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long VCPUsMax { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long VCPUsAtStartup { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public on_softreboot_behavior ActionsAfterSoftreboot { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public on_normal_exit ActionsAfterShutdown { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public on_normal_exit ActionsAfterReboot { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public on_crash_behaviour ActionsAfterCrash { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string PVBootloader { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string PVKernel { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string PVRamdisk { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string PVArgs { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string PVBootloaderArgs { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string PVLegacyArgs { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string HVMBootPolicy { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable HVMBootParams { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public double HVMShadowMultiplier { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable Platform { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string PCIBus { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable OtherConfig { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable LastBootCPUFlags { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string LastBootedRecord { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string Recommendations { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable XenstoreData { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool HaAlwaysRun { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string HaRestartPriority { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string[] Tags { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable BlockedOperations { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.VMPP> ProtectionPolicy { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool IsSnapshotFromVmpp { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.VMSS> SnapshotSchedule { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool IsVmssSnapshot { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.VM_appliance> Appliance { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long StartDelay { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long ShutdownDelay { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long Order { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public XenRef<XenAPI.SR> SuspendSR { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long Version { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string GenerationId { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public long HardwarePlatformVersion { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public bool HasVendorDevice { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public string ReferenceLabel { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public domain_type DomainType { get; set; }

        [Parameter(ParameterSetName = "Fields")]
        public Hashtable NVRAM { get; set; }

        protected override bool GenerateAsyncParam
        {
            get { return true; }
        }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            GetSession();
            if (Record == null && HashTable == null)
            {
                Record = new XenAPI.VM();
                if (MyInvocation.BoundParameters.ContainsKey("NameLabel"))
                    Record.name_label = NameLabel;
                if (MyInvocation.BoundParameters.ContainsKey("NameDescription"))
                    Record.name_description = NameDescription;
                if (MyInvocation.BoundParameters.ContainsKey("PowerState"))
                    Record.power_state = PowerState;
                if (MyInvocation.BoundParameters.ContainsKey("UserVersion"))
                    Record.user_version = UserVersion;
                if (MyInvocation.BoundParameters.ContainsKey("IsATemplate"))
                    Record.is_a_template = IsATemplate;
                if (MyInvocation.BoundParameters.ContainsKey("SuspendVDI"))
                    Record.suspend_VDI = new XenRef<XenAPI.VDI>(SuspendVDI == null ? "OpaqueRef:NULL" : SuspendVDI.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("Affinity"))
                    Record.affinity = new XenRef<XenAPI.Host>(Affinity == null ? "OpaqueRef:NULL" : Affinity.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("MemoryTarget"))
                    Record.memory_target = MemoryTarget;
                if (MyInvocation.BoundParameters.ContainsKey("MemoryStaticMax"))
                    Record.memory_static_max = MemoryStaticMax;
                if (MyInvocation.BoundParameters.ContainsKey("MemoryDynamicMax"))
                    Record.memory_dynamic_max = MemoryDynamicMax;
                if (MyInvocation.BoundParameters.ContainsKey("MemoryDynamicMin"))
                    Record.memory_dynamic_min = MemoryDynamicMin;
                if (MyInvocation.BoundParameters.ContainsKey("MemoryStaticMin"))
                    Record.memory_static_min = MemoryStaticMin;
                if (MyInvocation.BoundParameters.ContainsKey("VCPUsParams"))
                    Record.VCPUs_params = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(VCPUsParams);
                if (MyInvocation.BoundParameters.ContainsKey("VCPUsMax"))
                    Record.VCPUs_max = VCPUsMax;
                if (MyInvocation.BoundParameters.ContainsKey("VCPUsAtStartup"))
                    Record.VCPUs_at_startup = VCPUsAtStartup;
                if (MyInvocation.BoundParameters.ContainsKey("ActionsAfterSoftreboot"))
                    Record.actions_after_softreboot = ActionsAfterSoftreboot;
                if (MyInvocation.BoundParameters.ContainsKey("ActionsAfterShutdown"))
                    Record.actions_after_shutdown = ActionsAfterShutdown;
                if (MyInvocation.BoundParameters.ContainsKey("ActionsAfterReboot"))
                    Record.actions_after_reboot = ActionsAfterReboot;
                if (MyInvocation.BoundParameters.ContainsKey("ActionsAfterCrash"))
                    Record.actions_after_crash = ActionsAfterCrash;
                if (MyInvocation.BoundParameters.ContainsKey("PVBootloader"))
                    Record.PV_bootloader = PVBootloader;
                if (MyInvocation.BoundParameters.ContainsKey("PVKernel"))
                    Record.PV_kernel = PVKernel;
                if (MyInvocation.BoundParameters.ContainsKey("PVRamdisk"))
                    Record.PV_ramdisk = PVRamdisk;
                if (MyInvocation.BoundParameters.ContainsKey("PVArgs"))
                    Record.PV_args = PVArgs;
                if (MyInvocation.BoundParameters.ContainsKey("PVBootloaderArgs"))
                    Record.PV_bootloader_args = PVBootloaderArgs;
                if (MyInvocation.BoundParameters.ContainsKey("PVLegacyArgs"))
                    Record.PV_legacy_args = PVLegacyArgs;
                if (MyInvocation.BoundParameters.ContainsKey("HVMBootPolicy"))
                    Record.HVM_boot_policy = HVMBootPolicy;
                if (MyInvocation.BoundParameters.ContainsKey("HVMBootParams"))
                    Record.HVM_boot_params = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(HVMBootParams);
                if (MyInvocation.BoundParameters.ContainsKey("HVMShadowMultiplier"))
                    Record.HVM_shadow_multiplier = HVMShadowMultiplier;
                if (MyInvocation.BoundParameters.ContainsKey("Platform"))
                    Record.platform = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(Platform);
                if (MyInvocation.BoundParameters.ContainsKey("PCIBus"))
                    Record.PCI_bus = PCIBus;
                if (MyInvocation.BoundParameters.ContainsKey("OtherConfig"))
                    Record.other_config = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(OtherConfig);
                if (MyInvocation.BoundParameters.ContainsKey("LastBootCPUFlags"))
                    Record.last_boot_CPU_flags = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(LastBootCPUFlags);
                if (MyInvocation.BoundParameters.ContainsKey("LastBootedRecord"))
                    Record.last_booted_record = LastBootedRecord;
                if (MyInvocation.BoundParameters.ContainsKey("Recommendations"))
                    Record.recommendations = Recommendations;
                if (MyInvocation.BoundParameters.ContainsKey("XenstoreData"))
                    Record.xenstore_data = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(XenstoreData);
                if (MyInvocation.BoundParameters.ContainsKey("HaAlwaysRun"))
                    Record.ha_always_run = HaAlwaysRun;
                if (MyInvocation.BoundParameters.ContainsKey("HaRestartPriority"))
                    Record.ha_restart_priority = HaRestartPriority;
                if (MyInvocation.BoundParameters.ContainsKey("Tags"))
                    Record.tags = Tags;
                if (MyInvocation.BoundParameters.ContainsKey("BlockedOperations"))
                    Record.blocked_operations = CommonCmdletFunctions.ConvertHashTableToDictionary<vm_operations, string>(BlockedOperations);
                if (MyInvocation.BoundParameters.ContainsKey("ProtectionPolicy"))
                    Record.protection_policy = new XenRef<XenAPI.VMPP>(ProtectionPolicy == null ? "OpaqueRef:NULL" : ProtectionPolicy.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("IsSnapshotFromVmpp"))
                    Record.is_snapshot_from_vmpp = IsSnapshotFromVmpp;
                if (MyInvocation.BoundParameters.ContainsKey("SnapshotSchedule"))
                    Record.snapshot_schedule = new XenRef<XenAPI.VMSS>(SnapshotSchedule == null ? "OpaqueRef:NULL" : SnapshotSchedule.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("IsVmssSnapshot"))
                    Record.is_vmss_snapshot = IsVmssSnapshot;
                if (MyInvocation.BoundParameters.ContainsKey("Appliance"))
                    Record.appliance = new XenRef<XenAPI.VM_appliance>(Appliance == null ? "OpaqueRef:NULL" : Appliance.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("StartDelay"))
                    Record.start_delay = StartDelay;
                if (MyInvocation.BoundParameters.ContainsKey("ShutdownDelay"))
                    Record.shutdown_delay = ShutdownDelay;
                if (MyInvocation.BoundParameters.ContainsKey("Order"))
                    Record.order = Order;
                if (MyInvocation.BoundParameters.ContainsKey("SuspendSR"))
                    Record.suspend_SR = new XenRef<XenAPI.SR>(SuspendSR == null ? "OpaqueRef:NULL" : SuspendSR.opaque_ref);
                if (MyInvocation.BoundParameters.ContainsKey("Version"))
                    Record.version = Version;
                if (MyInvocation.BoundParameters.ContainsKey("GenerationId"))
                    Record.generation_id = GenerationId;
                if (MyInvocation.BoundParameters.ContainsKey("HardwarePlatformVersion"))
                    Record.hardware_platform_version = HardwarePlatformVersion;
                if (MyInvocation.BoundParameters.ContainsKey("HasVendorDevice"))
                    Record.has_vendor_device = HasVendorDevice;
                if (MyInvocation.BoundParameters.ContainsKey("ReferenceLabel"))
                    Record.reference_label = ReferenceLabel;
                if (MyInvocation.BoundParameters.ContainsKey("DomainType"))
                    Record.domain_type = DomainType;
                if (MyInvocation.BoundParameters.ContainsKey("NVRAM"))
                    Record.NVRAM = CommonCmdletFunctions.ConvertHashTableToDictionary<string, string>(NVRAM);
            }
            else if (Record == null)
            {
                Record = new XenAPI.VM(HashTable);
            }

            if (!ShouldProcess(session.Url, "VM.create"))
                return;

            RunApiCall(()=>
            {
                var contxt = _context as XenServerCmdletDynamicParameters;

                if (contxt != null && contxt.Async)
                {
                    taskRef = XenAPI.VM.async_create(session, Record);

                    if (PassThru)
                    {
                        XenAPI.Task taskObj = null;
                        if (taskRef != "OpaqueRef:NULL")
                        {
                            taskObj = XenAPI.Task.get_record(session, taskRef.opaque_ref);
                            taskObj.opaque_ref = taskRef.opaque_ref;
                        }

                        WriteObject(taskObj, true);
                    }
                }
                else
                {
                    string objRef = XenAPI.VM.create(session, Record);

                    if (PassThru)
                    {
                        XenAPI.VM obj = null;

                        if (objRef != "OpaqueRef:NULL")
                        {
                            obj = XenAPI.VM.get_record(session, objRef);
                            obj.opaque_ref = objRef;
                        }

                        WriteObject(obj, true);
                    }
                }

            });

            UpdateSessions();
        }

        #endregion
   }
}
