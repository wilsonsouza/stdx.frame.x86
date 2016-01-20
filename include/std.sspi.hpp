/**************************************************************************************************
*
* Copyright (c) 2013 Dedalus Prime. All Rights Reserveds
* DEDALUS PRIME PROPRIETARY
*
* Created by Wilson.Souza
*
*
*
*
*
*
*
* stl c++ interface
* Used to authentication by SSPI API Win32
*/
#pragma once
#define SECURITY_WIN32
#include <std.defs.hpp>
#include <tchar.h>
#include <sspi.h>
#include <lm.h>
#include <lmcons.h>
#include <string>

/*
// Older versions of WinError.h do not have SEC_I_COMPLETE_NEEDED #define.
// So, in such an SDK environment setup, we will include issperr.h which has the
// definition for SEC_I_COMPLETE_NEEDED. Include issperr.h only if
// SEC_I_COMPLETE_NEEDED is not defined.
*/
#ifndef SEC_I_COMPLETE_NEEDED
#include <issperr.h>
#endif
/* link library */
#pragma comment(lib, "netapi32.lib")
#pragma warning(disable:4251)

typedef struct _AUTH_SEQ 
{
   BOOL bInitialized;
   BOOL bHaveCredHandle;
   BOOL bHaveCtxtHandle;
   CredHandle hcred;
   struct _SecHandle hctxt;
} AUTH_SEQ, *PAUTH_SEQ;
/**/
typedef struct _DNS_DEVICE_INFO
{
   DWORD cbOffset;
   DWORD dwLength;
   WCHAR szName[10][PATHLEN];
   /**/
   typedef enum _DNS_DEVICE
   {
      NETBIOS,
      DNS_HOSTNAME,
      DNS_DOMAIN,
      DNS_FULLY_QUALIFIED,
      PHYSICAL_NETBIOS,
      PHYSICAL_DNS_HOSTNAME,
      PHYSICAL_DNS_DOMAIN,
      PHYSICAL_DNS_FULLY_QUALIFIED,
      DNS_UNKNOW = 0xff
   }DNS_DEVICE, *LPDNS_DEVICE;
   /**/
   BOOL bMode[10];
}DNS_DEVICE_INFO, *LPDNS_DEVICE_INFO;
/**/
namespace std
{
   /**/
   class _DYNAMICLINK sspi
   {
   public:
      sspi();
      virtual ~sspi();
      virtual LPVOID RetrieveTokenInformationClass(TOKEN_INFORMATION_CLASS InfoClass,
            LPDWORD lpdwSize);
      virtual PSID GetUserSidFromWellKnownRid(DWORD Rid);
      virtual BOOL IsGuest();
      virtual HMODULE LoadSecurityDynamicLibrary();
      virtual BOOL GenClientContext(PAUTH_SEQ pAS, 
         PSEC_WINNT_AUTH_IDENTITY pAuthIdentity, 
         PVOID pIn, DWORD cbIn, PVOID pOut, PDWORD pcbOut, PBOOL pfDone); 
      virtual BOOL GenServerContext(PAUTH_SEQ pAS, 
         PVOID pIn, DWORD cbIn, PVOID pOut, PDWORD pcbOut, PBOOL pfDone);
      virtual void const CheckAndLocalFree(HLOCAL hPtr);
      virtual BOOL Authenticate(wstring const & strDomain, 
         wstring const & strUser, wstring const & strPassword);
      virtual DNS_DEVICE_INFO GetComputerDomain();

   protected:
      virtual void const FillWinNTAuthIdentify(_Out_ SEC_WINNT_AUTH_IDENTITY * psai,
         wstring const & strDomain, wstring const & strUser, wstring const & strPassword);

   protected:
      /* function pointers */
      ACCEPT_SECURITY_CONTEXT_FN       m_AcceptSecurityContext;
      ACQUIRE_CREDENTIALS_HANDLE_FN    m_AcquireCredentialsHandle;
      COMPLETE_AUTH_TOKEN_FN           m_CompleteAuthToken;
      DELETE_SECURITY_CONTEXT_FN       m_DeleteSecurityContext;
      FREE_CONTEXT_BUFFER_FN           m_FreeContextBuffer;
      FREE_CREDENTIALS_HANDLE_FN       m_FreeCredentialsHandle;
      INITIALIZE_SECURITY_CONTEXT_FN   m_InitializeSecurityContext;
      QUERY_SECURITY_PACKAGE_INFO_FN   m_QuerySecurityPackageInfo;
      QUERY_SECURITY_CONTEXT_TOKEN_FN  m_QuerySecurityContextToken;
      /* token handle */
      HANDLE m_hToken;
      HMODULE m_hModule;
      /* domain */
      wstring m_strHost;
   };
   //-----------------------------------------------------------------------------------------------//
   __inline void const sspi::CheckAndLocalFree(HLOCAL hPtr)
   {
      if(hPtr != nullptr)
      {
         LocalFree(hPtr);
         hPtr = nullptr;
      }
   }
   //-----------------------------------------------------------------------------------------------//
};