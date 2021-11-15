#ifndef _DISTANCE_NATIVE_API_HPP_
#define _DISTANCE_NATIVE_API_HPP_

#include <Windows.h>
#include <winternl.h>

#pragma comment(lib, "ntdll")

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096u
#endif

//-----------------------------------------------------------------------------
// Macro RTL_CONSTANT_STRING from ntdef.h
//-----------------------------------------------------------------------------
char _RTL_CONSTANT_STRING_type_check(const char* s);
char _RTL_CONSTANT_STRING_type_check(const WCHAR* s);
// __typeof would be desirable here instead of sizeof.
template <size_t N> 
class _RTL_CONSTANT_STRING_remove_const_template_class;

template <> 
class _RTL_CONSTANT_STRING_remove_const_template_class<sizeof(char)> 
{ public: typedef  char T; };

template <> 
class _RTL_CONSTANT_STRING_remove_const_template_class<sizeof(WCHAR)> 
{ public: typedef WCHAR T; };

#define _RTL_CONSTANT_STRING_remove_const_macro(s) \
(const_cast<_RTL_CONSTANT_STRING_remove_const_template_class<sizeof((s)[0])>::T*>(s))

#define RTL_CONSTANT_STRING(s) \
{ \
    sizeof( s ) - sizeof( (s)[0] ), \
    sizeof( s ) / sizeof(_RTL_CONSTANT_STRING_type_check(s)), \
    _RTL_CONSTANT_STRING_remove_const_macro(s) \
}
//-----------------------------------------------------------------------------


#define NtCurrentProcess() ((HANDLE)(LONG_PTR) -1)

typedef enum _SECTION_INFORMATION_CLASS {
    SectionBasicInformation,
    SectionImageInformation
} SECTION_INFORMATION_CLASS, *PSECTION_INFORMATION_CLASS;

typedef enum _SECTION_INHERIT {
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT, *PSECTION_INHERIT;



//-----------------------------------------------------------------------------
// NT Section
//-----------------------------------------------------------------------------
EXTERN_C_START

NTSYSAPI NTSTATUS NTAPI NtCreateSection(
    OUT PHANDLE SectionHandle,
    IN ULONG DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN PLARGE_INTEGER MaximumSize OPTIONAL,
    IN ULONG PageAttributess,
    IN ULONG SectionAttributes,
    IN HANDLE FileHandle OPTIONAL
);



NTSYSAPI NTSTATUS NTAPI NtOpenSection(
    OUT PHANDLE SectionHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes
);



NTSYSAPI NTSTATUS NTAPI NtMapViewOfSection(
    IN HANDLE SectionHandle,
    IN HANDLE ProcessHandle,
    IN OUT PVOID* BaseAddress OPTIONAL,
    IN ULONG_PTR ZeroBits OPTIONAL,
    IN SIZE_T CommitSize,
    IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
    IN OUT PSIZE_T ViewSize,
    IN SECTION_INHERIT InheritDisposition,
    IN ULONG AllocationType OPTIONAL,
    IN ULONG Protect
);



NTSYSAPI NTSTATUS NTAPI NtUnmapViewOfSection(
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress
);



NTSYSAPI NTSTATUS NTAPI NtQuerySection(
    IN HANDLE SectionHandle,
    IN SECTION_INFORMATION_CLASS InformationClass,
    OUT PVOID InformationBuffer,
    IN ULONG InformationBufferSize,
    OUT PULONG ResultLength OPTIONAL
);



NTSYSAPI NTSTATUS NTAPI NtExtendSection(
    IN HANDLE SectionHandle,
    IN PLARGE_INTEGER NewSectionSize
);

EXTERN_C_END
//-----------------------------------------------------------------------------

#endif // _DISTANCE_NATIVE_API_HPP_