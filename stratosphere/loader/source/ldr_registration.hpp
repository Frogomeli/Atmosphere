#pragma once
#include <switch.h>

#include "ldr_map.hpp"

#define REGISTRATION_LIST_MAX (0x40)

#define NSO_INFO_MAX (0x20)
#define NRR_INFO_MAX (0x40)
#define NRO_INFO_MAX (0x40)

class Registration {
    public:
        struct NsoInfo {
            u64 base_address;
            u64 size;
            unsigned char build_id[0x20];
        };
        
        struct NsoInfoHolder {
            bool in_use;
            NsoInfo info;
        };
        
        struct NroInfo {
            bool in_use;
            u64 base_address;
            u64 total_mapped_size;
            u64 nro_heap_address;
            u64 nro_heap_size;
            u64 bss_heap_address;
            u64 bss_heap_size;
            u64 text_size;
            u64 ro_size;
            u64 rw_size;
            unsigned char build_id[0x20];
        };
        
        struct TidSid {
            u64 title_id;
            FsStorageId storage_id;
        };
        
        struct Process {
            bool in_use;
            bool is_64_bit_addspace;
            u64 index;
            u64 process_id;
            u64 title_id;
            Registration::TidSid tid_sid;
            Registration::NsoInfoHolder nso_infos[NSO_INFO_MAX];
            Registration::NroInfo nro_infos[NRO_INFO_MAX];
            MappedCodeMemory nrr_infos[NRR_INFO_MAX];
            void *owner_ro_service;
        };
        
        struct List {
            Registration::Process processes[REGISTRATION_LIST_MAX];
            u64 num_processes;
        };
        
        static Registration::Process *GetFreeProcess();
        static Registration::Process *GetProcess(u64 index);
        static Registration::Process *GetProcessByProcessId(u64 pid);
        static Registration::Process *GetProcessByRoService(void *service);
        static Result GetRegisteredTidSid(u64 index, Registration::TidSid *out);
        static bool RegisterTidSid(const TidSid *tid_sid, u64 *out_index);
        static bool UnregisterIndex(u64 index);
        static void SetProcessIdTidAndIs64BitAddressSpace(u64 index, u64 process_id, u64 tid, bool is_64_bit_addspace);
        static void AddNsoInfo(u64 index, u64 base_address, u64 size, const unsigned char *build_id);
        static void CloseRoService(void *service, Handle process_h);
        static Result AddNrrInfo(u64 index, MappedCodeMemory *nrr_info);
        static Result RemoveNrrInfo(u64 index, u64 base_address);
        static bool IsNroHashPresent(u64 index, u8 *nro_hash);
        static bool IsNroAlreadyLoaded(u64 index, u8 *build_id);
        static void AddNroToProcess(u64 index, MappedCodeMemory *nro, MappedCodeMemory *bss, u32 text_size, u32 ro_size, u32 rw_size, u8 *build_id);
        static Result RemoveNroInfo(u64 index, Handle process_h, u64 base_address);
        static Result GetNsoInfosForProcessId(NsoInfo *out, u32 max_out, u64 process_id, u32 *num_written);
        
        /* Atmosphere MitM Extension. */
        static void AssociatePidTidForMitM(u64 index);
};
