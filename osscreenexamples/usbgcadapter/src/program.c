/* Really quick and dirty usb gamecube adapter pad 1 print-out */
#include "program.h"

typedef struct
{
    unsigned int handle;
    unsigned int physical_device_inst;
    unsigned short vid;
    unsigned short pid;
    unsigned char interface_index;
    unsigned char sub_class;
    unsigned char protocol;

    unsigned short max_packet_size_rx;
    unsigned short max_packet_size_tx;

} HIDDevice;

typedef struct _HIDClient HIDClient;

#define HID_DEVICE_DETACH	0
#define HID_DEVICE_ATTACH	1

typedef int (*HIDAttachCallback)(HIDClient *p_hc,HIDDevice *p_hd,unsigned int attach);

struct _HIDClient
{
    HIDClient *next;
    HIDAttachCallback attach_cb;
};

typedef void (*HIDCallback)(unsigned int handle,int error,unsigned char *p_buffer,unsigned int bytes_transferred,void *p_user);

#define DEVICE_UNUSED 0
#define DEVICE_USED 1

static int my_attach_cb(HIDClient *p_client, HIDDevice *p_device, unsigned int attach);

void _entryPoint()
{
    /****************************>           Get Handles           <****************************/
    //Get a handle to coreinit.rpl
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
    unsigned int nsyshid_handle;
    OSDynLoad_Acquire("nsyshid.rpl", &nsyshid_handle);
    /****************************>       External Prototypes       <****************************/
    //OS functions
    void(*_Exit)();
    int(*HIDAddClient)(HIDClient *p_client, HIDAttachCallback attach_callback);
    int(*HIDDelClient)(HIDClient *p_client);
    /****************************>             Exports             <****************************/
    //OS functions
    OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);

    OSDynLoad_FindExport(nsyshid_handle, 0, "HIDAddClient", &HIDAddClient);
    OSDynLoad_FindExport(nsyshid_handle, 0, "HIDDelClient", &HIDDelClient);

    HIDClient fd;
    HIDAddClient(&fd, my_attach_cb);
    while(1) ;
    HIDDelClient(&fd);
    //WARNING: DO NOT CHANGE THIS. YOU MUST CLEAR THE FRAMEBUFFERS AND IMMEDIATELY CALL EXIT FROM THIS FUNCTION. RETURNING TO LOADER CAUSES FREEZE.
    int ii=0;
    for(ii; ii<2; ii++)
    {
        fillScreen(0,0,0,0);
        flipBuffers();
    }
    _Exit();
}

static void charprintf(char *msg)
{
    int i=0;
    for(i; i<2; i++)
    {
        fillScreen(0,0,0,0);
        drawString(0,0,msg);
        flipBuffers();
    }
}

struct my_cb_user {
    unsigned char *buf;
    unsigned int nsyshid_handle;
    void *HIDRead;
    void *HIDWrite;
    unsigned int transfersize;
};

static void my_read_cb(unsigned int handle, int error, unsigned char *buf, unsigned int bytes_transfered, void *p_user)
{
    if(error == 0)
    {
        struct my_cb_user *usr = (struct my_cb_user*)p_user;
        char leprint[64];
        __os_snprintf(leprint,64,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9]);
        charprintf(leprint);
        int(*HIDRead)(unsigned int handle, unsigned char *p_buffer, unsigned int buffer_length, HIDCallback hc, void *p_user) = usr->HIDRead;
        HIDRead(handle, usr->buf, bytes_transfered, my_read_cb, usr);
    }
}

static void my_write_cb(unsigned int handle, int error, unsigned char *buf, unsigned int bytes_transfered, void *p_user)
{
    if(error == 0)
    {
        struct my_cb_user *usr = (struct my_cb_user*)p_user;
        charprintf("adapter started");
        int(*HIDRead)(unsigned int handle, unsigned char *p_buffer, unsigned int buffer_length, HIDCallback hc, void *p_user) = usr->HIDRead;
        HIDRead(handle, usr->buf, usr->transfersize, my_read_cb, usr);
    }
}

#define SWAP16(x) ((x>>8) | ((x&0xFF)<<8))
static int my_attach_cb(HIDClient *p_client, HIDDevice *p_device, unsigned int attach)
{
    char buf[64];
    __os_snprintf(buf,64,"%s:%04x %04x",attach?"attach":"detach",SWAP16(p_device->vid),SWAP16(p_device->pid));
    charprintf(buf);
    if( attach && SWAP16(p_device->vid) == 0x057e && SWAP16(p_device->pid) == 0x0337 )
    {
        unsigned int coreinit_handle;
        OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
        void*(*OSAllocFromSystem)(unsigned int v1, unsigned int v2);
        OSDynLoad_FindExport(coreinit_handle,0,"OSAllocFromSystem",&OSAllocFromSystem);
        void(*memset)(void *dst, unsigned char wat, unsigned int len);
        OSDynLoad_FindExport(coreinit_handle,0,"memset",&memset);
        unsigned char *buf = OSAllocFromSystem(64,64);
        memset(buf,0,64);
        buf[0] = 0x13;
        struct my_cb_user *usr = OSAllocFromSystem(64,64);
        usr->buf = buf;
        unsigned int nsyshid_handle;
        OSDynLoad_Acquire("nsyshid.rpl", &nsyshid_handle);
        usr->nsyshid_handle = nsyshid_handle;
        OSDynLoad_FindExport(usr->nsyshid_handle,0,"HIDRead",&usr->HIDRead);
        OSDynLoad_FindExport(usr->nsyshid_handle,0,"HIDWrite",&usr->HIDWrite);
        usr->transfersize = p_device->max_packet_size_rx;
        int(*HIDWrite)(unsigned int handle, unsigned char *p_buffer, unsigned int buffer_length, HIDCallback hc, void *p_user) = usr->HIDWrite;
        HIDWrite(p_device->handle, usr->buf, 1, my_write_cb, usr);
        return DEVICE_USED;
    }
    return DEVICE_UNUSED;
}
