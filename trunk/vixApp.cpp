#include "vixApp.h"

/*
   Template code:
VixError killProcessesInGuest(VixHandle vmHandle,
                              uint64 pid,
                              int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);


   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // killProcessesInGuest function ends here

*/

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
   printf("Error [%d]: %s: %s\n", (int) err, debugString, errorText);
   return;
} // printErrorText

VixError connectEx(VixHandle *hostHandle,
                   int apiversion,
                   VixServiceProvider serviceProvider,
                   const char *hostname,
                   int hostport,
                   const char *username,
                   const char *password,
                   int options)
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
                               options, // options
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
} //connectEx

VixError disconnect(VixHandle hostHandle)
{
   VixError err = VIX_OK;  
   
   printFunctionCallMessage(__FUNCTION__);
   
   VixHost_Disconnect(hostHandle);
   
   printSuccessMessage(__FUNCTION__);
   
   return err;
}// disconnnect


VixError powerOn(VixHandle hostHandle,
                 VixHandle *vmHandle,
                 const char *vmxFilePath)
{
   return powerOnEx(hostHandle, vmHandle, vmxFilePath,
                    VIX_VMPOWEROP_NORMAL);
}

VixError powerOnEx(VixHandle hostHandle,
                   VixHandle *vmHandle,
                   const char *vmxFilePath,
                   VixVMPowerOpOptions powerOptions)
{                  
   VixHandle jobHandle;
   VixError err;
   Bool jobCompleted = FALSE;

   printFunctionCallMessage(__FUNCTION__);
   err = justOpen(hostHandle, vmHandle, vmxFilePath);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);
      goto abort;
   }

   jobHandle = VixVM_PowerOn(*vmHandle,
                             powerOptions, // powerOnOptions,
                             VIX_INVALID_HANDLE, // propertyListHandle,
                             NULL, // callbackProc,
                             NULL); // clientData
/*   while (!jobCompleted) {
      err = VixJob_CheckCompletion(jobHandle,
                                   &jobCompleted);
      if (VIX_OK != err) {
        printErrorText(err, __FUNCTION__);
        goto abort;
      }      
   }
*/ 
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

VixError justOpen(VixHandle hostHandle,
                  VixHandle *vmHandle,
                  const char *vmxFilePath)
{
   VixHandle jobHandle;
   VixError err;
   Bool jobCompleted = FALSE;

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

   printSuccessMessage(__FUNCTION__);

abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
}// justOpen


VixError justOpenVM(VixHandle hostHandle,
                    VixHandle *vmHandle,
                    const char *vmxFilePath,
                    VixHandle propertyListHandle)
{
   VixHandle jobHandle = VIX_INVALID_HANDLE;
   VixError err;
   Bool jobCompleted = FALSE;
 
   printFunctionCallMessage(__FUNCTION__);
   jobHandle = VixHost_OpenVM(hostHandle,
                              vmxFilePath,
                              0,
                              propertyListHandle,
                              NULL,
                              NULL);
   
   err = VixJob_Wait(jobHandle,
                     VIX_PROPERTY_JOB_RESULT_HANDLE,
                     vmHandle,
                     VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);
      goto abort;
   }

   printSuccessMessage(__FUNCTION__);

abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
}// justOpenVM

VixError powerOff(VixHandle hostHandle,
                  const char *vmxFilePath)
{
   return powerOffEx(hostHandle, vmxFilePath, VIX_VMPOWEROP_NORMAL);

} // powerOff function ends here

VixError powerOffEx(VixHandle hostHandle,
                    const char *vmxFilePath,
                    VixVMPowerOpOptions powerOptions)
{
   VixHandle jobHandle = VIX_INVALID_HANDLE;
   VixHandle vmHandle;
   VixError err;
   
   printFunctionCallMessage(__FUNCTION__);
   err = justOpen(hostHandle, &vmHandle, vmxFilePath);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);
      goto abort;
   }

   jobHandle = VixVM_PowerOff(vmHandle,
                             powerOptions, // powerOffOptions,
                             NULL,  // callbackProc,
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

VixError loginToGuestEx(VixHandle vmHandle,
                        const char *username,
                        const char *password,
                        int options,
                        int timeout)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__); 
   printf("Timeout specified %d\n", timeout);
   jobHandle = VixVM_WaitForToolsInGuest(vmHandle,
                                         timeout,
                                         NULL, // callbackProc
                                         NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, "vix job wait after wait for tools");
      goto abort;
   }

   Vix_ReleaseHandle(jobHandle);
  
   // Authenticate for guest operations.
   jobHandle = VixVM_LoginInGuest(vmHandle,
                                  username, // userName
                                  password, // password
                                  options, // options
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

VixError loginToGuest(VixHandle vmHandle,
                      const char *username,
                      const char *password,
                      int options)
{
   return loginToGuestEx(vmHandle,
                         username,
                         password,
                         options,
                         DEFAULT_TIME_OUT);
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
   jobHandle = VixVM_CopyFileFromHostToGuest(vmHandle,
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
   jobHandle = VixVM_DeleteFileInGuest(vmHandle,
                                            filePath,
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
   Bool directoryExists;
   VixPropertyType propertyType;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_DirectoryExistsInGuest(vmHandle,
                                            directoryPath,
                                            NULL,
                                            NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__ );      
      goto abort;
   }
   err = Vix_GetProperties(jobHandle,
                           VIX_PROPERTY_JOB_RESULT_GUEST_OBJECT_EXISTS,
                           &directoryExists,
                           VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }

   if (directoryExists) {
      printf(" Directory %s exists in VM \n", directoryPath);
   } else {
      printf(" Directory %s doesn't exist in VM\n", directoryPath);
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //directoryExistsInGuest

VixError captureScreenImage(VixHandle vmHandle,
                            int captureType,
                            const char *screenImageFilePath)
{
   VixHandle jobHandle;
   VixError err;
   int byte_count;
   void *screen_bits;
   FILE *fp;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_CaptureScreenImage(vmHandle,
                               VIX_CAPTURESCREENFORMAT_PNG,
                               VIX_INVALID_HANDLE,
                               NULL,
                               NULL);
   err = VixJob_Wait(jobHandle,
                     VIX_PROPERTY_JOB_RESULT_SCREEN_IMAGE_DATA,
                     &byte_count, &screen_bits,
                     VIX_PROPERTY_NONE);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }

   printf("got image back, %d bytes\n", byte_count);
   fp = fopen(screenImageFilePath, "wb+");
   if (fp) {
      fwrite(screen_bits, byte_count, 1, fp);
      fclose(fp);
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_FreeBuffer(screen_bits);
   Vix_ReleaseHandle(jobHandle);
   return err;

} // captureScreenImage

VixError enableSharedFoldersInGuest(VixHandle vmHandle,
                                    Bool enable,
                                    int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_EnableSharedFolders(vmHandle,
                                         enable,
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
} //eanbleSharedFoldersInGuest

VixError fileExistsInGuest(VixHandle vmHandle,
                           const char *filePath)
{
   VixHandle jobHandle;
   VixError err;
   Bool fileExists;

   printFunctionCallMessage(__FUNCTION__);   
   jobHandle = VixVM_FileExistsInGuest(vmHandle,
                                       filePath,
                                       NULL,
                                       NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   err = Vix_GetProperties(jobHandle,
                           VIX_PROPERTY_JOB_RESULT_GUEST_OBJECT_EXISTS,
                           &fileExists,
                           VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   if (fileExists) {
      printf(" File %s exists in VM \n", filePath);
   } else {
      printf(" File %s doesn't exist in VM\n", filePath);
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //directoryExistsInGuest

VixError getFileInfoInGuest(VixHandle vmHandle,
                            const char *filePath)
{
   VixHandle jobHandle;
   VixError err;
   int fileFlags;
   int64 fileSize;

   printFunctionCallMessage(__FUNCTION__);
   jobHandle = VixVM_GetFileInfoInGuest(vmHandle,
                                        filePath, 
                                        NULL,
                                        NULL);
   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      fprintf(stderr, "failed to get file info for '%s'(%"FMT64"d %s)\n",
              filePath, err, Vix_GetErrorText(err, NULL));
      goto abort;
   }

   err = Vix_GetProperties(jobHandle,
                           VIX_PROPERTY_JOB_RESULT_FILE_FLAGS,
                           &fileFlags,
                           VIX_PROPERTY_JOB_RESULT_FILE_SIZE,
                           &fileSize,
                           VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printf(" File %s: Flags are %d and file Size is %d\n", filePath, 
                                                          fileFlags,
                                                          fileSize);
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // getFileInfoInGuest function ends here


VixError installToolsInGuest(VixHandle vmHandle,
                             int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   jobHandle = VixVM_InstallTools(vmHandle,
                                  options, // options
                                  NULL, // commandLineArgs
                                  NULL, // callbackProc
                                  NULL); // clientData

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   printf("Error for VixVM_Install Tools is %d\n", err);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }

   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //installToolsInGuest function ends here

VixError killProcessInGuest(VixHandle vmHandle,
                              uint64 pid,
                              int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   jobHandle = VixVM_KillProcessInGuest(vmHandle,
                                        pid,
                                        options,
                                        NULL,
                                        NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // killProcessInGuest function ends here

VixError listDirectoryInGuest(VixHandle vmHandle,
                                const char *directoryPath,
                                int options)
{
   VixHandle jobHandle;
   VixError err;
   int num, i;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_ListDirectoryInGuest(vmHandle, directoryPath,
                                          options, NULL, NULL);
   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);
      goto abort;
   }

   num = VixJob_GetNumProperties(jobHandle, VIX_PROPERTY_JOB_RESULT_ITEM_NAME);
   for (i = 0; i < num; i++) {
      char *fname;

      err = VixJob_GetNthProperties(jobHandle,
                                    i,
                                    VIX_PROPERTY_JOB_RESULT_ITEM_NAME,
                                    &fname,
                                    VIX_PROPERTY_NONE);

      printf("file #%d '%s'\n", i, fname);

      Vix_FreeBuffer(fname);
   }

   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // listDirectoryInGuest function ends here

VixError listProcessesInGuest(VixHandle vmHandle,
                              int options)
{
   VixHandle jobHandle;
   VixError err;
   int num, i;
   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_ListProcessesInGuest(vmHandle, options, NULL, NULL);
   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   num = VixJob_GetNumProperties(jobHandle, VIX_PROPERTY_JOB_RESULT_ITEM_NAME);
   for (i = 0; i < num; i++) {
      char *processName;
      uint64 pid;
      char *owner;
      char *cmdline;

      err = VixJob_GetNthProperties(jobHandle, i,
                        VIX_PROPERTY_JOB_RESULT_ITEM_NAME, &processName,
                        VIX_PROPERTY_JOB_RESULT_PROCESS_ID, &pid,
                        VIX_PROPERTY_JOB_RESULT_PROCESS_OWNER, &owner,
                        VIX_PROPERTY_JOB_RESULT_PROCESS_COMMAND, &cmdline,
                        VIX_PROPERTY_NONE);

      printf("process #%d '%s' %"FMT64"d %s %s\n",
             i, processName, pid, owner, cmdline);

      Vix_FreeBuffer(processName);
      Vix_FreeBuffer(owner);
      Vix_FreeBuffer(cmdline);
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // listProcessesInGuest function ends here

VixError openURLInGuest(VixHandle vmHandle,
                        const char *url,
                        int windowState,
                        VixHandle propertyListHandle)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_OpenUrlInGuest(vmHandle, url, windowState,
                                    propertyListHandle, 
                                    NULL,
                                    NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // OpenUrlInGuest function ends here

VixError renameFileInGuest(VixHandle vmHandle,
                           const char *oldFilePath,
                           const char *newFilePath,
                           int options,
                           VixHandle propertyListHandle)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_RenameFileInGuest(vmHandle, oldFilePath,
                                       newFilePath, options, 
                                       propertyListHandle,
                                       NULL,
                                       NULL);
   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // renameFileInGuest function ends here

VixError runProgramInGuest(VixHandle vmHandle,
                           const char *programName,
                           const char *commandLineArgs)
{
   VixHandle jobHandle;
   VixError err;
   VixRunProgramOptions options;

   printFunctionCallMessage(__FUNCTION__);
   options = VIX_RUNPROGRAM_RETURN_IMMEDIATELY;
   jobHandle = VixVM_RunProgramInGuest(vmHandle, programName,
                                       commandLineArgs,
                                       options,
                                       VIX_INVALID_HANDLE,
                                       NULL,
                                       NULL);
   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // runProgramInGuest function ends here

VixError runScriptInGuest(VixHandle vmHandle,
                          const char *interpreter,
                          const char *scriptText)
{
   VixHandle jobHandle;
   VixError err;
   VixRunProgramOptions options;

   printFunctionCallMessage(__FUNCTION__);
   options = VIX_RUNPROGRAM_RETURN_IMMEDIATELY;
   jobHandle = VixVM_RunScriptInGuest(vmHandle,
                                      interpreter,
                                      scriptText,
                                      options,
                                      VIX_INVALID_HANDLE,
                                      NULL,
                                      NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // RunScriptInGuest function ends here

VixError pauseVM(VixHandle vmHandle,
                 int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_Pause(vmHandle,
                           options,
                           VIX_INVALID_HANDLE,
                           NULL,
                           NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // pauseVM function ends here

VixError unpauseVM(VixHandle vmHandle,
                 int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_Unpause(vmHandle,
                             options,
                             VIX_INVALID_HANDLE,
                             NULL,
                             NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // unpauseVM function ends here

VixError resetVM(VixHandle vmHandle, VixVMPowerOpOptions powerOptions)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_Reset(vmHandle,
                           powerOptions,
                           NULL,
                           NULL);
   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // resetVM function ends here

VixError suspendVM(VixHandle vmHandle)
{
   VixHandle jobHandle;
   VixError err;
   VixVMPowerOpOptions poweroffOptions = VIX_RUNPROGRAM_RETURN_IMMEDIATELY;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_Suspend(vmHandle,
                             0,
                             NULL,
                             NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // cloneVM function ends here

VixError cloneVM(VixHandle vmHandle,
                 VixHandle snapshotHandle,
                 VixCloneType cloneType,
                 const char *destConfigPath,
                 int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_Clone(vmHandle,
                           snapshotHandle,
                           cloneType,
                           destConfigPath,
                           options,
                           NULL,
                           NULL,
                           NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // suspendVM function ends here

VixError readVariableInVM(VixHandle vmHandle,
                          int variableType,
                          const char *variableName,
                          int options)
{
   VixHandle jobHandle;
   VixError err;
   char *readValue = NULL;
   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_ReadVariable(vmHandle,
                                  variableType,
                                  variableName,
                                  options,
                                  NULL,
                                  NULL);

   err = VixJob_Wait(jobHandle, 
                     VIX_PROPERTY_JOB_RESULT_VM_VARIABLE_STRING,
                     &readValue,
                     VIX_PROPERTY_NONE);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   printf(" Value of variable %s in Guest is %s\n", variableName, readValue);
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // readVariableInVM function ends here

VixError writeVariableInVM(VixHandle vmHandle,
                          int variableType,
                          const char *variableName,
                          const char *variableValue,
                          int options)
{
   VixHandle jobHandle;
   VixError err;
   char *readValue = NULL;
   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_WriteVariable(vmHandle,
                                  variableType,
                                  variableName,
                                  variableValue,
                                  options,
                                  NULL,
                                  NULL);

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
} // writeVariableInVM function ends here

VixError createSnapshotInGuest(VixHandle vmHandle,
                               const char *name,
                               const char *description,
                               int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   jobHandle = VixVM_CreateSnapshot(vmHandle,
                                    name,
                                    description,
                                    options,
                                    VIX_INVALID_HANDLE,
                                    NULL,
                                    NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // createSnapshotInGuest function ends here

VixError removeSnapshotInGuest(VixHandle vmHandle,
                               VixHandle snapshotHandle,
                               int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   jobHandle = VixVM_RemoveSnapshot(vmHandle,
                                    snapshotHandle,
                                    options,
                                    NULL,
                                    NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // removeSnapshotInGuest function ends here

VixError revertToSnapshotInGuest(VixHandle vmHandle,
                                 VixHandle snapshotHandle,
                                 int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   jobHandle = VixVM_RevertToSnapshot(vmHandle,
                                      snapshotHandle,
                                      options,
                                      VIX_INVALID_HANDLE,
                                      NULL,
                                      NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // revertToSnapshotInGuest function ends here

VixError getCurrentSnapshotInGuest(VixHandle vmHandle,
                                   VixHandle *snapshotHandle)
{
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   err = VixVM_GetCurrentSnapshot(vmHandle,
                                        snapshotHandle);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   return err;
} // getCurrentSnapshotInGuest function ends here

VixError getNamedSnapshotInGuest(VixHandle vmHandle,
                                 const char *snapshotName,
                                 VixHandle *snapshotHandle)
{
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   err = VixVM_GetNamedSnapshot(vmHandle,
                                        snapshotName,
                                        snapshotHandle);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   return err;
} // getCurrentSnapshotInGuest function ends here

VixError getRootSnapshotInGuest(VixHandle vmHandle,
                                int index,
                                VixHandle *snapshotHandle)
{
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   err = VixVM_GetRootSnapshot(vmHandle,
                               index,
                               snapshotHandle);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   return err;
} // getRootSnapshotInGuest function ends here

VixError getNumberRootSnapshotInGuest(VixHandle vmHandle,
                                      int *result)
{
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   err = VixVM_GetNumRootSnapshots(vmHandle,
                                   result);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   return err;
} // getNumRootSnapshotInGuest function ends here

VixError getParentSnapshotInGuest(VixHandle snapshotHandle,
                                  VixHandle *parentSnapshotHandle)
{
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   err = VixSnapshot_GetParent(snapshotHandle,
                               parentSnapshotHandle);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   return err;
} // getParentSnapshotInGuest function ends here

VixError getNumChildrenInGuest(VixHandle snapshotHandle,
                               int *childCount)
{
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   err = VixSnapshot_GetNumChildren(snapshotHandle,
                                    childCount);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   return err;
} // getNumChildrenInGuest function ends here

VixError getChildSnapshotInGuest(VixHandle snapshotHandle,
                                 int childIndex,
                                 VixHandle *childSnapshotHandle)
{
   VixError err;

   printFunctionCallMessage(__FUNCTION__);
   err = VixSnapshot_GetChild(snapshotHandle,
                                    childIndex,
                                    childSnapshotHandle);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   return err;
} // getChildSnapshotInGuest function ends here

VixError beginRecordingInGuest(VixHandle vmHandle,
                               char *recordingName,
                               char *recordingDescription,
                               int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_BeginRecording(vmHandle, recordingName,
                                    recordingDescription,
                                    options,
                                    VIX_INVALID_HANDLE,
                                    NULL,
                                    NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // beginRecordingInGuest function ends here

VixError endRecordingInGuest(VixHandle vmHandle,
                             int options)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_EndRecording(vmHandle,
                                  options,
                                  VIX_INVALID_HANDLE,
                                  NULL,
                                  NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // endRecordingInGuest function ends here

VixError addSharedFolderInGuest(VixHandle vmHandle,
                                const char *shareName,
                                const char *hostPathName,
                                VixMsgSharedFolderOptions flags)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_AddSharedFolder(vmHandle,
                                     shareName,
                                     hostPathName,
                                     flags,
                                     NULL,
                                     NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // addSharedFolderInGuest function ends here

VixError removeSharedFolderInGuest(VixHandle vmHandle,
                                   const char *shareName,
                                   VixMsgSharedFolderOptions flags)
{
   VixHandle jobHandle;
   VixError err;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_RemoveSharedFolder(vmHandle,
                                        shareName,
                                        flags,
                                        NULL,
                                        NULL);

   err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // removeSharedFolderInGuest function ends here

VixError getNumSharedFoldersInGuest(VixHandle vmHandle)
{
   VixHandle jobHandle;
   VixError err;
   int sharedFolderCount;

   printFunctionCallMessage(__FUNCTION__);

   jobHandle = VixVM_GetNumSharedFolders(vmHandle,
                                         NULL,
                                         NULL);

   err = VixJob_Wait(jobHandle, 
                     VIX_PROPERTY_JOB_RESULT_SHARED_FOLDER_COUNT,
                     &sharedFolderCount);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }
   
   printf(" Number of Shared folders in vm is %d \n", sharedFolderCount);
   printSuccessMessage(__FUNCTION__);
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // getNumSharedFoldersInGuest function ends here

VixError getSharedFolderState(VixHandle vmHandle)
{
   VixHandle jobHandle;
   VixError err;
   int numSharedFolders;
   char *folderName;
   char *folderHostPath;
   int folderFlags;
   int i;

   jobHandle = VixVM_GetNumSharedFolders(vmHandle, NULL, NULL);
   err = VixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_SHARED_FOLDER_COUNT, 
		     &numSharedFolders, VIX_PROPERTY_NONE);
   Vix_ReleaseHandle(jobHandle);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }

   for (i = 0; i < numSharedFolders; i++) {
      jobHandle = VixVM_GetSharedFolderState(vmHandle, i, NULL, NULL);
      err = VixJob_Wait(jobHandle,
                        VIX_PROPERTY_JOB_RESULT_ITEM_NAME, &folderName,
                        VIX_PROPERTY_JOB_RESULT_SHARED_FOLDER_HOST, &folderHostPath,
                        VIX_PROPERTY_JOB_RESULT_SHARED_FOLDER_FLAGS, &folderFlags,
		        VIX_PROPERTY_NONE);
      if (VIX_OK != err) {
         printErrorText(err, __FUNCTION__);      
         goto abort;
      }
      Vix_ReleaseHandle(jobHandle);
      printf("Folder #%d: %s:%s, flags %d\n",
                     i, folderHostPath, folderName, folderFlags);
      Vix_FreeBuffer(folderName);
      Vix_FreeBuffer(folderHostPath);
   }
abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} // getSharedFolderState function ends here 

VixError setSharedFolderState(VixHandle vmHandle)
{
   VixHandle jobHandle;
   VixError err;
   int numSharedFolders;
   char *folderName;
   char *folderHostPath;
   int folderFlags;
   int i;

   jobHandle = VixVM_GetNumSharedFolders(vmHandle, NULL, NULL);
   err = VixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_SHARED_FOLDER_COUNT, 
		     &numSharedFolders, VIX_PROPERTY_NONE);
   Vix_ReleaseHandle(jobHandle);

   if (VIX_OK != err) {
      printErrorText(err, __FUNCTION__);      
      goto abort;
   }

   for (i = 0; i < numSharedFolders; i++) {
      jobHandle = VixVM_GetSharedFolderState(vmHandle, i, NULL, NULL);
      err = VixJob_Wait(jobHandle,
                        VIX_PROPERTY_JOB_RESULT_ITEM_NAME, &folderName,
                        VIX_PROPERTY_JOB_RESULT_SHARED_FOLDER_HOST, &folderHostPath,
                        VIX_PROPERTY_JOB_RESULT_SHARED_FOLDER_FLAGS, &folderFlags,
		        VIX_PROPERTY_NONE);
      if (VIX_OK != err) {
         printErrorText(err, __FUNCTION__);      
         Vix_ReleaseHandle(jobHandle);
         goto abort;
      }
      printf("Folder #%d: %s:%s, flags %d\n", i, folderHostPath, folderName, folderFlags);

      /*
       * make all folders writable
       */
      jobHandle = VixVM_SetSharedFolderState(vmHandle, folderName, folderHostPath,
                                             VIX_SHAREDFOLDER_WRITE_ACCESS,
                                             NULL, NULL);

      err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
      if (VIX_OK != err) {
         printErrorText(err, __FUNCTION__);      
         Vix_ReleaseHandle(jobHandle);
         goto abort;
      }

      Vix_ReleaseHandle(jobHandle);
      Vix_FreeBuffer(folderName);
      Vix_FreeBuffer(folderHostPath);
   }
abort:
   return err;
} // getSharedFolderState function ends here 

void captureText(void)
{
   char ch;
   printf("Enter a character:");
   scanf("%c", &ch);
   return;
} // captureText function ends here