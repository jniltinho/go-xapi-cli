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


using System.Management.Automation;
using XenAPI;

namespace Citrix.XenServer.Commands
{
    [Cmdlet(VerbsData.ConvertTo, "XenRef")]
    [OutputType(typeof(IXenObject))]
    public class ConvertToXenRefCommand : PSCmdlet
    {
        #region Cmdlet Parameters

        [Parameter(Mandatory = true, ValueFromPipeline = true, Position = 0,
            HelpMessage = "The API object to convert")]
        public IXenObject XenObject { get; set; }

        #endregion

        #region Cmdlet Methods

        protected override void ProcessRecord()
        {
            if (XenObject is Session session)
            {
                WriteObject(new XenRef<Session>(session));
                return;
            }
            if (XenObject is Auth auth)
            {
                WriteObject(new XenRef<Auth>(auth));
                return;
            }
            if (XenObject is Subject subject)
            {
                WriteObject(new XenRef<Subject>(subject));
                return;
            }
            if (XenObject is Role role)
            {
                WriteObject(new XenRef<Role>(role));
                return;
            }
            if (XenObject is Task task)
            {
                WriteObject(new XenRef<Task>(task));
                return;
            }
            if (XenObject is Event evt)
            {
                WriteObject(new XenRef<Event>(evt));
                return;
            }
            if (XenObject is Pool pool)
            {
                WriteObject(new XenRef<Pool>(pool));
                return;
            }
            if (XenObject is Pool_patch pool_patch)
            {
                WriteObject(new XenRef<Pool_patch>(pool_patch));
                return;
            }
            if (XenObject is Pool_update pool_update)
            {
                WriteObject(new XenRef<Pool_update>(pool_update));
                return;
            }
            if (XenObject is VM vm)
            {
                WriteObject(new XenRef<VM>(vm));
                return;
            }
            if (XenObject is VM_metrics vm_metrics)
            {
                WriteObject(new XenRef<VM_metrics>(vm_metrics));
                return;
            }
            if (XenObject is VM_guest_metrics vm_guest_metrics)
            {
                WriteObject(new XenRef<VM_guest_metrics>(vm_guest_metrics));
                return;
            }
            if (XenObject is VMPP vmpp)
            {
                WriteObject(new XenRef<VMPP>(vmpp));
                return;
            }
            if (XenObject is VMSS vmss)
            {
                WriteObject(new XenRef<VMSS>(vmss));
                return;
            }
            if (XenObject is VM_appliance vm_appliance)
            {
                WriteObject(new XenRef<VM_appliance>(vm_appliance));
                return;
            }
            if (XenObject is DR_task dr_task)
            {
                WriteObject(new XenRef<DR_task>(dr_task));
                return;
            }
            if (XenObject is Host host)
            {
                WriteObject(new XenRef<Host>(host));
                return;
            }
            if (XenObject is Host_crashdump host_crashdump)
            {
                WriteObject(new XenRef<Host_crashdump>(host_crashdump));
                return;
            }
            if (XenObject is Host_patch host_patch)
            {
                WriteObject(new XenRef<Host_patch>(host_patch));
                return;
            }
            if (XenObject is Host_metrics host_metrics)
            {
                WriteObject(new XenRef<Host_metrics>(host_metrics));
                return;
            }
            if (XenObject is Host_cpu host_cpu)
            {
                WriteObject(new XenRef<Host_cpu>(host_cpu));
                return;
            }
            if (XenObject is Network network)
            {
                WriteObject(new XenRef<Network>(network));
                return;
            }
            if (XenObject is VIF vif)
            {
                WriteObject(new XenRef<VIF>(vif));
                return;
            }
            if (XenObject is VIF_metrics vif_metrics)
            {
                WriteObject(new XenRef<VIF_metrics>(vif_metrics));
                return;
            }
            if (XenObject is PIF pif)
            {
                WriteObject(new XenRef<PIF>(pif));
                return;
            }
            if (XenObject is PIF_metrics pif_metrics)
            {
                WriteObject(new XenRef<PIF_metrics>(pif_metrics));
                return;
            }
            if (XenObject is Bond bond)
            {
                WriteObject(new XenRef<Bond>(bond));
                return;
            }
            if (XenObject is VLAN vlan)
            {
                WriteObject(new XenRef<VLAN>(vlan));
                return;
            }
            if (XenObject is SM sm)
            {
                WriteObject(new XenRef<SM>(sm));
                return;
            }
            if (XenObject is SR sr)
            {
                WriteObject(new XenRef<SR>(sr));
                return;
            }
            if (XenObject is Sr_stat sr_stat)
            {
                WriteObject(new XenRef<Sr_stat>(sr_stat));
                return;
            }
            if (XenObject is Probe_result probe_result)
            {
                WriteObject(new XenRef<Probe_result>(probe_result));
                return;
            }
            if (XenObject is LVHD lvhd)
            {
                WriteObject(new XenRef<LVHD>(lvhd));
                return;
            }
            if (XenObject is VDI vdi)
            {
                WriteObject(new XenRef<VDI>(vdi));
                return;
            }
            if (XenObject is VBD vbd)
            {
                WriteObject(new XenRef<VBD>(vbd));
                return;
            }
            if (XenObject is VBD_metrics vbd_metrics)
            {
                WriteObject(new XenRef<VBD_metrics>(vbd_metrics));
                return;
            }
            if (XenObject is PBD pbd)
            {
                WriteObject(new XenRef<PBD>(pbd));
                return;
            }
            if (XenObject is Crashdump crashdump)
            {
                WriteObject(new XenRef<Crashdump>(crashdump));
                return;
            }
            if (XenObject is VTPM vtpm)
            {
                WriteObject(new XenRef<VTPM>(vtpm));
                return;
            }
            if (XenObject is Console console)
            {
                WriteObject(new XenRef<Console>(console));
                return;
            }
            if (XenObject is User user)
            {
                WriteObject(new XenRef<User>(user));
                return;
            }
            if (XenObject is Data_source data_source)
            {
                WriteObject(new XenRef<Data_source>(data_source));
                return;
            }
            if (XenObject is Blob blob)
            {
                WriteObject(new XenRef<Blob>(blob));
                return;
            }
            if (XenObject is Message message)
            {
                WriteObject(new XenRef<Message>(message));
                return;
            }
            if (XenObject is Secret secret)
            {
                WriteObject(new XenRef<Secret>(secret));
                return;
            }
            if (XenObject is Tunnel tunnel)
            {
                WriteObject(new XenRef<Tunnel>(tunnel));
                return;
            }
            if (XenObject is Network_sriov network_sriov)
            {
                WriteObject(new XenRef<Network_sriov>(network_sriov));
                return;
            }
            if (XenObject is PCI pci)
            {
                WriteObject(new XenRef<PCI>(pci));
                return;
            }
            if (XenObject is PGPU pgpu)
            {
                WriteObject(new XenRef<PGPU>(pgpu));
                return;
            }
            if (XenObject is GPU_group gpu_group)
            {
                WriteObject(new XenRef<GPU_group>(gpu_group));
                return;
            }
            if (XenObject is VGPU vgpu)
            {
                WriteObject(new XenRef<VGPU>(vgpu));
                return;
            }
            if (XenObject is VGPU_type vgpu_type)
            {
                WriteObject(new XenRef<VGPU_type>(vgpu_type));
                return;
            }
            if (XenObject is PVS_site pvs_site)
            {
                WriteObject(new XenRef<PVS_site>(pvs_site));
                return;
            }
            if (XenObject is PVS_server pvs_server)
            {
                WriteObject(new XenRef<PVS_server>(pvs_server));
                return;
            }
            if (XenObject is PVS_proxy pvs_proxy)
            {
                WriteObject(new XenRef<PVS_proxy>(pvs_proxy));
                return;
            }
            if (XenObject is PVS_cache_storage pvs_cache_storage)
            {
                WriteObject(new XenRef<PVS_cache_storage>(pvs_cache_storage));
                return;
            }
            if (XenObject is Feature feature)
            {
                WriteObject(new XenRef<Feature>(feature));
                return;
            }
            if (XenObject is SDN_controller sdn_controller)
            {
                WriteObject(new XenRef<SDN_controller>(sdn_controller));
                return;
            }
            if (XenObject is Vdi_nbd_server_info vdi_nbd_server_info)
            {
                WriteObject(new XenRef<Vdi_nbd_server_info>(vdi_nbd_server_info));
                return;
            }
            if (XenObject is PUSB pusb)
            {
                WriteObject(new XenRef<PUSB>(pusb));
                return;
            }
            if (XenObject is USB_group usb_group)
            {
                WriteObject(new XenRef<USB_group>(usb_group));
                return;
            }
            if (XenObject is VUSB vusb)
            {
                WriteObject(new XenRef<VUSB>(vusb));
                return;
            }
            if (XenObject is Cluster cluster)
            {
                WriteObject(new XenRef<Cluster>(cluster));
                return;
            }
            if (XenObject is Cluster_host cluster_host)
            {
                WriteObject(new XenRef<Cluster_host>(cluster_host));
                return;
            }
            if (XenObject is Certificate certificate)
            {
                WriteObject(new XenRef<Certificate>(certificate));
                return;
            }
            if (XenObject is Repository repository)
            {
                WriteObject(new XenRef<Repository>(repository));
                return;
            }
            if (XenObject is Observer observer)
            {
                WriteObject(new XenRef<Observer>(observer));
                return;
            }
            if (XenObject is VM_group vm_group)
            {
                WriteObject(new XenRef<VM_group>(vm_group));
                return;
            }
            if (XenObject is Host_driver host_driver)
            {
                WriteObject(new XenRef<Host_driver>(host_driver));
                return;
            }
            if (XenObject is Driver_variant driver_variant)
            {
                WriteObject(new XenRef<Driver_variant>(driver_variant));
                return;
            }
        }

        #endregion
    }
}