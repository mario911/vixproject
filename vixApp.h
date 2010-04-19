#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "vix.h"

#ifndef _VIX_APP_H
#define _VIX_APP_H

#define DEFAULT_TIME_OUT 300

void printFunctionCallMessage(const char *debugString);
void printSuccessMessage(const char *debugString);
void printErrorText(VixError err, const char *debugString);

/*
 * Auth related functions
 * Also host related functions
 */
VixError connectEx(VixHandle *hostHandle,
                   int apiversion,
                   VixServiceProvider serviceProvider,
                   const char *hostname,
                   int hostport,
                   const char *username,
                   const char *password,
                   int options);
VixError loginToGuest(VixHandle vmHandle,
                      const char *username,
                      const char *password,
                      int options);
VixError loginToGuestEx(VixHandle vmHandle,
                        const char *username,
                        const char *password,
                        int options,
                        int timeoutInSeconds);
VixError logoutFromGuest(VixHandle vmHandle);
VixError disconnect(VixHandle hostHandle);

/* Console related function */
VixError captureScreenImage(VixHandle vmHandle,
                            int captureType,
                            const char *screenImageFilePath);

/* Find related function */

/* Guest related function */
VixError copyFileFromGuestToHost(VixHandle vmHandle,
                                 const char *guestFilePath,
                                 const char *hostFilePath,
                                 int options);
VixError copyFileFromHostToGuest(VixHandle vmHandle,
                                 const char *hostFilePath,
                                 const char *guestFilePath,
                                 int options);
VixError createDirectoryInGuest(VixHandle vmHandle,
                                const char *directoryPath);
VixError createTempFileInGuest(VixHandle vmHandle,
                               int options);
VixError deleteDirectoryInGuest(VixHandle vmHandle,
                               const char *directoryPath);
VixError deleteFileInGuest(VixHandle vmHandle,
                           const char *filePath,
                           int options);
VixError directoryExistsInGuest(VixHandle vmHandle,
                                const char *directoryPath);
VixError enableSharedFoldersInGuest(VixHandle vmHandle,
                                    Bool enable,
                                    int options);
VixError fileExistsInGuest(VixHandle vmHandle,
                           const char *filePath);
VixError getFileInfoInGuest(VixHandle vmHandle,
                            const char *filePath);
VixError installToolsInGuest(VixHandle vmHandle,
                             int options);
VixError killProcessInGuest(VixHandle vmHandle,
                              uint64 pid,
                              int options);
VixError listDirectoryInGuest(VixHandle vmHandle,
                                const char *directoryPath,
                                int options);
VixError listProcessesInGuest(VixHandle vmHandle,
                              int options);
VixError openURLInGuest(VixHandle vmHandle,
                        const char *url,
                        int windowState,
                        VixHandle propertyListHandle);
VixError renameFileInGuest(VixHandle vmHandle,
                           const char *oldFilePath,
                           const char *newFilePath,
                           int options,
                           VixHandle propertyListHandle);
VixError runProgramInGuest(VixHandle vmHandle,
                           const char *programName,
                           const char *commandLineArgs);
VixError runScriptInGuest(VixHandle vmHandle,
                          const char *interpreter,
                          const char *scriptText);

/* Power related functions */
VixError powerOn(VixHandle hostHandle,
                 VixHandle *vmHandle,
                 const char *vmxFilePath);
VixError powerOnEx(VixHandle hostHandle,
                   VixHandle *vmHandle,
                   const char *vmxFilePath,
                   VixVMPowerOpOptions powerOptions);
VixError powerOff(VixHandle hostHandle,
                  const char *vmxFilePath);
VixError powerOffEx(VixHandle hostHandle,
                    const char *vmxFilePath,
                    VixVMPowerOpOptions powerOptions);
VixError pauseVM(VixHandle vmHandle,
                 int options);
VixError unpauseVM(VixHandle vmHandle,
                 int options);
VixError resetVM(VixHandle vmHandle, VixVMPowerOpOptions powerOptions);

/* VM related functions */
VixError justOpen(VixHandle hostHandle,
                  VixHandle *vmHandle,
                  const char *vmxFilePath);
VixError justOpenVM(VixHandle hostHandle,
                    VixHandle *vmHandle,
                    const char *vmxFile,
                    VixHandle propertyListHandle);
VixError suspendVM(VixHandle vmHandle);
VixError cloneVM(VixHandle vmHandle,
                 VixHandle snapshotHandle,
                 VixCloneType cloneType,
                 const char *destConfigPath,
                 int options);
VixError readVariableInVM(VixHandle vmHandle,
                          int variableType,
                          const char *variableName,
                          int options);
VixError writeVariableInVM(VixHandle vmHandle,
                          int variableType,
                          const char *variableName,
                          const char *variableValue,
                          int options);

/* Snapshot related functions */
VixError createSnapshotInGuest(VixHandle vmHandle,
                               const char *name,
                               const char *description,
                               int options);
VixError removeSnapshotInGuest(VixHandle vmHandle,
                               VixHandle snapshotHandle,
                               int options);
VixError revertToSnapshotInGuest(VixHandle vmHandle,
                                 VixHandle snapshotHandle,
                                 int options);
VixError getCurrentSnapshotInGuest(VixHandle vmHandle,
                                   VixHandle *snapshotHandle);
VixError getNamedSnapshotInGuest(VixHandle vmHandle,
                                 const char *snapshotName,
                                 VixHandle *snapshotHandle);
VixError getRootSnapshotInGuest(VixHandle vmHandle,
                                int index,
                                VixHandle *snapshotHandle);
VixError getNumberRootSnapshotInGuest(VixHandle vmHandle,
                                      int *result);
VixError getParentSnapshotInGuest(VixHandle snapshotHandle,
                                  VixHandle *parentSnapshotHandle);
VixError getNumChildrenInGuest(VixHandle snapshotHandle,
                               int *childCount);
VixError getChildSnapshotInGuest(VixHandle snapshotHandle,
                                 int childIndex,
                                 VixHandle *childSnapshotHandle);
VixError addSharedFolderInGuest(VixHandle vmHandle,
                                const char *shareName,
                                const char *hostPathName,
                                VixMsgSharedFolderOptions flags);
VixError removeSharedFolderInGuest(VixHandle vmHandle,
                                   const char *shareName,
                                   VixMsgSharedFolderOptions flags);
VixError getNumSharedFoldersInGuest(VixHandle vmHandle);
VixError getSharedFolderState(VixHandle vmHandle);
VixError setSharedFolderState(VixHandle vmHandle);

/* Record/Replay related functions */
VixError beginRecordingInGuest(VixHandle vmHandle,
                               char *recordingName,
                               char *recordingDescription,
                               int options);
VixError endRecordingInGuest(VixHandle vmHandle,
                             int options);

void captureText(void);

#endif