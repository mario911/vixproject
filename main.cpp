#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "vix.h"

#define DEFAULT_TIME_OUT 300

int  apiversion;
VixServiceProvider serviceProvider;
char *hostname;
int hostport;
char *username;
char *password;
char *vmxFilePath;
char *guestUsername;
char *guestPassword;
int options;

void printFunctionCallMessage(const char *debugString)
{
   printf("%s API() Called\n", debugString);
} //printFunctionCallMessage

void printSuccessMessage(const char *debugString)
{
   printf("%s API() Succeeded\n", debugString);
   return;
} //printSuccessMessage
void printErrorText(VixError err,
                    const char *debugString)
{
   const char *errorText = NULL;
   errorText = Vix_GetErrorText(err, NULL);
   printf("Error: %s: %s\n", debugString, errorText);
   return;
} // printErrorText

VixError connect(VixHandle *hostHandle)
{
   VixHandle jobHandle = VIX_INVALID_HANDLE;
   VixError err = VIX_OK;

   printFunctionCallMessage(__FUNCTION__);
   jobHandle = VixHost_Connect(apiversion,
                               serviceProvider,
                               hostname, // hostName
                               hostport, // hostPort
                               username, // userName
                               password, // password,
                               0, // options
                               VIX_INVALID_HANDLE, // propertyListHandle
                               NULL, // callbackProc
                               NULL); // clientData   
   
   err = VixJob_Wait(jobHandle,
                     VIX_PROPERTY_JOB_RESULT_HANDLE,
                     hostHandle,
                     VIX_PROPERTY_NONE);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);
      goto abort;
    }
   printSuccessMessage(__FUNCTION__);

abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //connect

VixError disconnect(VixHandle hostHandle)
{
   VixError err = VIX_OK;  
   
   printFunctionCallMessage(__FUNCTION__);
   
   VixHost_Disconnect(hostHandle);
   
   printSuccessMessage(__FUNCTION__);
   
   return err;
}// disconnnect

VixError powerOn(VixHandle hostHandle,
                 VixHandle *vmHandle)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   jobHandle = VixVM_Open(hostHandle,
                       vmxFilePath,
                       NULL, // callbackProc
                       NULL); // clientData
   err = VixJob_Wait(jobHandle,
                     VIX_PROPERTY_JOB_RESULT_HANDLE,
                     vmHandle,
                     VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);
      goto abort;
   }
   Vix_ReleaseHandle(jobHandle);
   jobHandle = VixVM_PowerOn(*vmHandle,
                             VIX_VMPOWEROP_NORMAL, // powerOnOptions,
                             VIX_INVALID_HANDLE, // propertyListHandle,
                             NULL, // callbackProc,
                             NULL); // clientData

   err = VixJob_Wait(jobHandle,
                  VIX_PROPERTY_NONE);

   if (VIX_OK != err) {
     printErrorText(err, __FUNCTION__);
     goto abort;
   }
   printSuccessMessage(__FUNCTION__);

abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
}// powerOn

VixError powerOff(VixHandle hostHandle)
{
   VixHandle jobHandle;
   VixHandle vmHandle;
   VixError err;
   
   printFunctionCallMessage(__FUNCTION__);
   jobHandle = VixVM_Open(hostHandle,
                       vmxFilePath,
                       NULL, // callbackProc
                       NULL); // clientData
   err = VixJob_Wait(jobHandle,
                     VIX_PROPERTY_JOB_RESULT_HANDLE,
                     &vmHandle,
                     VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);
      goto abort;
   }
   Vix_ReleaseHandle(jobHandle);
   jobHandle = VixVM_PowerOff(vmHandle,
                             VIX_VMPOWEROP_FROM_GUEST, // powerOffOptions,
                             NULL, // callbackProc,
                             NULL); // clientData

   err = VixJob_Wait(jobHandle,
                  VIX_PROPERTY_NONE);

   if (VIX_OK != err) {
     printErrorText(err, __FUNCTION__);
     goto abort;
   }
   printSuccessMessage(__FUNCTION__);

abort:
   Vix_ReleaseHandle(jobHandle);
   Vix_ReleaseHandle(vmHandle);
   return err;   
} // powerOff

VixError loginToGuest(VixHandle vmHandle,
                      const char *username,
                      const char *password,
                      int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_WaitForToolsInGuest(vmHandle,
                                         DEFAULT_TIME_OUT, // timeoutInSeconds
                                         NULL, // callbackProc
                                         NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }

   Vix_ReleaseHandle(jobHandle);

   // Authenticate for guest operations.
   jobHandle = VixVM_LoginInGuest(vmHandle,
                                  username, // userName
                                  password, // password
                                  0, // options
                                  NULL, // callbackProc
                                  NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }

   Vix_ReleaseHandle(jobHandle);   
   printSuccessMessage(__FUNCTION__);
abort:
   return err;
} // loginToGuest

VixError logoutFromGuest(VixHandle vmHandle)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_LogoutFromGuest(vmHandle,
                                  NULL, // callbackProc
                                  NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //logoutFromGuest

VixError copyFileFromGuestToHost(VixHandle vmHandle,
                                 const char *guestFilePath,
                                 const char *hostFilePath,
                                 int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_CopyFileFromGuestToHost(vmHandle,
                                             guestFilePath, // callbackProc
                                             hostFilePath,
                                             options,
                                             VIX_INVALID_HANDLE,
                                             NULL,
                                             NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //copyFileFromHostToGuest 
VixError copyFileFromHostToGuest(VixHandle vmHandle,
                                 const char *hostFilePath,
                                 const char *guestFilePath,
                                 int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_CopyFileFromGuestToHost(vmHandle,
                                             hostFilePath,
                                             guestFilePath,
                                             options,
                                             VIX_INVALID_HANDLE,
                                             NULL,
                                             NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //copyFileFromHostToGuest 

VixError createDirectoryInGuest(VixHandle vmHandle,
                                const char *directoryPath)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_CreateDirectoryInGuest(vmHandle,
                                            directoryPath,
                                            VIX_INVALID_HANDLE,
                                            NULL,
                                            NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //copyFileFromGuestToHost 

VixError createTempFileInGuest(VixHandle vmHandle,
                               int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_CreateTempFileInGuest(vmHandle,
                                           options,
                                           VIX_INVALID_HANDLE,
                                           NULL,
                                           NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //createTempFileInGuest
VixError deleteDirectoryInGuest(VixHandle vmHandle,
                               const char *directoryPath)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_DeleteDirectoryInGuest(vmHandle,
                                            directoryPath,
                                            VIX_INVALID_HANDLE,
                                            NULL,
                                            NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //deleteDirectoryInGuest
VixError deleteFileInGuest(VixHandle vmHandle,
                           const char *filePath,
                           int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_DeleteDirectoryInGuest(vmHandle,
                                            filePath,
                                            options,
                                            NULL,
                                            NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //deleteFileInGuest
VixError directoryExistsInGuest(VixHandle vmHandle,
                                const char *directoryPath)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_DirectoryExistsInGuest(vmHandle,
                                            direcotryPath,
                                            NULL,
                                            NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //directoryExistsInGuest

int main(int argc, char * argv[])
{
   VixHandle hostHandle = VIX_INVALID_HANDLE;
   VixHandle vmHandle = VIX_INVALID_HANDLE;
   VixError err = VIX_OK;
   
   apiversion = -1;
   serviceProvider = VIX_SERVICEPROVIDER_VMWARE_VI_SERVER;
   hostname = "https://ipaddress/sdk";
   hostport = 902;
   username = "root";
   password = "password";
   guestUsername = "";
   guestPassword = "";
   vmxFilePath = "/path/to/vmx/file";
   options = 0;

   connect(&hostHandle);
   powerOn(hostHandle, &vmHandle);
   err = loginToGuest(vmHandle, guestUsername, guestPassword, options);   

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }

   logoutFromGuest(vmHandle);
abort:
   Vix_ReleaseHandle(vmHandle);
   powerOff(hostHandle);
   disconnect(hostHandle);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);
      return -1;
   }
   
   return 0;
}
