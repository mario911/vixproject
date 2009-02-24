#include <stdio.h>
#include <stdlib.h>
#include "vix.h"

int  apiversion;
VixServiceProvider serviceProvider;
char *hostname;
int hostport;
char *username;
char *password;
char *vmxFilePath;

VixError connect(VixHandle *hostHandle)
{
   VixHandle jobHandle = VIX_INVALID_HANDLE;
   VixError err = VIX_OK;
   printf("connect() API called \n");
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
      printf("connect() API failed with error %d \n", err);
      goto abort;
    }
   printf("connect() API succeeded \n");

abort:
   Vix_ReleaseHandle(jobHandle);
   return err;
} //connect

VixError disconnect(VixHandle hostHandle)
{
   VixError err = VIX_OK;  
   
   VixHost_Disconnect(hostHandle);
   printf("disconnect() API succeeded \n");
   
   return err;
}// disconnnect

int main(int argc, char * argv[])
{
   VixHandle hostHandle = VIX_INVALID_HANDLE;
   VixError err = VIX_OK;

   apiversion = -1;
   serviceProvider = VIX_SERVICEPROVIDER_VMWARE_VI_SERVER;
   hostname = "https://10.20.84.131/sdk";
   username = "root";
   password = "";
   vmxFilePath = NULL;

   connect(&hostHandle);

   disconnect(hostHandle);


   if (VIX_OK != err) {
      return -1;
   }
   return 0;
}
