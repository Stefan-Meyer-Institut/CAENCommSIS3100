#include"CAENComm.h"

#include"sis3100_vme_calls.h"
#include "dev/pci/sis1100_var.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
#ifndef SISDEVICE
#define SISDEVICE "/dev/sis1100_03remote"
#endif

#ifdef __cplusplus
extern "C" {
#endif
  
  uint32_t vmebase;

  CAENComm_ErrorCode STDCALL CAENComm_DecodeError(int ErrCode, char *ErrMsg){
    switch(ErrCode) {
    case CAENComm_Success:
      strcpy(ErrMsg,"Operation completed successfully");
    case CAENComm_VMEBusError:
      strcpy(ErrMsg,"VME bus error during the cycle");
    case CAENComm_CommError:
      strcpy(ErrMsg,"Communication error");
    case CAENComm_GenericError:
      strcpy(ErrMsg,"Unspecified error");
    case CAENComm_InvalidParam:
      strcpy(ErrMsg,"Invalid parameter");
    case CAENComm_InvalidLinkType:
      strcpy(ErrMsg,"Invalid Link type");
    case CAENComm_InvalidHandler:
      strcpy(ErrMsg,"Invalid device handler");
    case CAENComm_CommTimeout:
      strcpy(ErrMsg,"Communication Timeout");
    case CAENComm_DeviceNotFound:
      strcpy(ErrMsg,"Unable to Open the requested Device");
    case CAENComm_MaxDevicesError:
      strcpy(ErrMsg,"Maximum number of devices exceeded");
    case CAENComm_DeviceAlreadyOpen:
      strcpy(ErrMsg,"The device is already opened");
    case CAENComm_NotSupported:
      strcpy(ErrMsg,"Not supported function");
    case CAENComm_UnusedBridge:
      strcpy(ErrMsg,"There aren't board controlled by that CAEN Bridge");
    case CAENComm_Terminated:
      strcpy(ErrMsg,"Communication terminated by the Device");
    default:
      strcpy(ErrMsg,"Unknown error code");
      return CAENComm_GenericError;
    }
    return 0;
  }

  CAENComm_ErrorCode STDCALL CAENComm_OpenDevice(CAENComm_ConnectionType LinkType, int LinkNum, int ConetNode, uint32_t VMEBaseAddress, int *handle){

    printf("\nhandle = %i \nadr= 0x%X\n",*handle,vmebase);
    *handle = open(SISDEVICE, O_RDWR, 0);
    vmebase = VMEBaseAddress;
    printf("\nhandle = %i \nadr= 0x%X\n",*handle,vmebase);
    if(*handle < 0)
      return CAENComm_DeviceNotFound;
    else
      return CAENComm_Success;

    /* ignore LinkType, LinkNum, ConetNode and VMEBaseAddress*/
    LinkType;
    LinkNum;
    ConetNode;
    VMEBaseAddress;
  }

  CAENComm_ErrorCode STDCALL CAENComm_CloseDevice(int handle){
    close(handle);
    return CAENComm_Success;
  }

  CAENComm_ErrorCode STDCALL CAENComm_Write32(int handle, uint32_t Address, uint32_t Data){
    int retcode = vme_A32D32_write(handle, vmebase+Address, Data);
    printf("handle=%i adr=0x%X, Data=%i retocde=%i\n",handle, vmebase+Address, Data, retcode);
    if(retcode!=0)
      return CAENComm_CommError;
    else
      return CAENComm_Success;
    
  }

  CAENComm_ErrorCode STDCALL CAENComm_Write16(int handle, uint32_t Address, uint16_t Data){
    int retcode = vme_A32D16_write(handle, vmebase+Address, Data);
    /*printf("handle=%i adr=0x%X, Data=%i retocde=%i\n",handle, vmebase+Address, Data, retcode);*/
    /*     printf("Write16: base=0x%X adr=0x%X\n",vmebase, Address);*/
    if(retcode!=0)
      return CAENComm_CommError;
    else
      return CAENComm_Success;
  }

  CAENComm_ErrorCode STDCALL CAENComm_Read32(int handle, uint32_t Address, uint32_t *Data){
    int retcode = vme_A32D32_read(handle, vmebase+Address, Data);

    /*printf("\nhandle = %i \nRETCODE: %i\n adress: 0x%X\n",handle, retcode, vmebase+Address);*/
    if(retcode!=0)
      return CAENComm_CommError;
    else
      return CAENComm_Success;
  }

  CAENComm_ErrorCode STDCALL CAENComm_Read16(int handle, uint32_t Address, uint16_t *Data){
    int retcode = vme_A32D16_read(handle, vmebase+Address, Data);
    if(retcode!=0)
      return CAENComm_CommError;
    else
      return CAENComm_Success;
  }

  CAENComm_ErrorCode STDCALL CAENComm_MultiRead32(int handle, uint32_t *Address, int nCycles, uint32_t *data, CAENComm_ErrorCode *ErrorCode){
    int i=0;
    for(;i<nCycles;i++){
      ErrorCode[i] = CAENComm_Read32(handle,Address[i],(&data[i]));
    }
    return  CAENComm_Success;
  }


  CAENComm_ErrorCode STDCALL CAENComm_MultiRead16(int handle, uint32_t *Address, int nCycles, uint16_t *data, CAENComm_ErrorCode *ErrorCode){
    int i=0;
    for(;i<nCycles;i++){
      ErrorCode[i] = CAENComm_Read16(handle,Address[i],(&data[i]));
    }
    return  CAENComm_Success;
  }
  
  CAENComm_ErrorCode STDCALL CAENComm_MultiWrite16(int handle, uint32_t *Address, int nCycles, uint16_t *data, CAENComm_ErrorCode *ErrorCode){
    int i=0;
    for(;i<nCycles;i++){
      /*printf("base=0x%X adr=0x%X\n",vmebase, Address[i]);*/
      ErrorCode[i] = CAENComm_Write16(handle,Address[i],(data[i]));
      printf("Err: %i Adr: 0x%X data: %i\n", ErrorCode[i], Address[i], data[i] );

    }
    return  CAENComm_Success;
  }


  CAENComm_ErrorCode STDCALL CAENComm_MultiWrite32(int handle, uint32_t *Address, int nCycles, uint32_t *data, CAENComm_ErrorCode *ErrorCode){
    int i=0;
    for(;i<nCycles;i++){
      ErrorCode[i] = CAENComm_Write32(handle,Address[i],(data[i]));
      printf("Err: %i \nAdr: 0x%X\n data: %i", ErrorCode[i], Address[i], data[i] );
    }
    return  CAENComm_Success;
  }


  CAENComm_ErrorCode STDCALL CAENComm_BLTRead(int handle, uint32_t Address, uint32_t *Buff, int BltSize, int *nw){
    int r = vme_A32BLT32_read(handle,vmebase+Address,Buff,BltSize,nw);
    if(r<0) return CAENComm_CommError;
    else return CAENComm_Success;
  }

  CAENComm_ErrorCode STDCALL CAENComm_MBLTRead(int handle, uint32_t Address, uint32_t *Buff, int BltSize, int *nw){
    int r = vme_A32MBLT64_read(handle,vmebase+Address,Buff,BltSize,nw);
    if(r<0) return CAENComm_CommError;
    else return CAENComm_Success;
  }

  CAENComm_ErrorCode STDCALL CAENComm_VMEIRQCheck(int VMEhandle, uint8_t *Mask){
    return 0;
  }

  CAENComm_ErrorCode STDCALL CAENComm_IRQDisable(int handle){
    return 0;
  }

  CAENComm_ErrorCode STDCALL CAENComm_IRQEnable(int handle){
    return 0;
  }


  CAENComm_ErrorCode STDCALL CAENComm_VMEIACKCycle16(int VMEhandle, IRQLevels Level, int *BoardID){
    return 0;
  }

  CAENComm_ErrorCode STDCALL CAENComm_VMEIACKCycle32(int VMEhandle, IRQLevels Level, int *BoardID){
    return 0;
  }


  CAENComm_ErrorCode STDCALL CAENComm_IACKCycle(int handle, IRQLevels Level, int *BoardID){
    return 0;
  }

  CAENComm_ErrorCode STDCALL CAENComm_IRQWait(int handle, uint32_t Timeout){
    return 0;
  }

  CAENComm_ErrorCode STDCALL CAENComm_VMEIRQWait(CAENComm_ConnectionType LinkType, int LinkNum, int ConetNode, uint8_t IRQMask, uint32_t Timeout, int *VMEHandle){
    return 0;																			}

  CAENComm_ErrorCode STDCALL CAENComm_Info(int handle, CAENCOMM_INFO info, void *data){
    return 0;
  }

  CAENComm_ErrorCode STDCALL CAENComm_SWRelease(char *SwRel){
    strcpy(SwRel,"0");
    return 0;
  }

#ifdef __cplusplus
}
#endif
