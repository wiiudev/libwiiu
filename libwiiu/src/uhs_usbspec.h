#ifndef UHS_USBSPEC
#define UHS_USBSPEC

/* USB class codes */
#define USBCLASS_DEVICE		0x00
#define USBCLASS_AUDIO		0x01
#define USBCLASS_HID		0x03
#define USBCLASS_STORAGE	0x08

/* USB device descriptor */
typedef struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUsb;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubclass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
} __attribute__((packed)) UhsDeviceDescriptor;
 
/* USB configuration descriptor */
typedef struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t bMaxPower;
} __attribute__((packed)) UhsConfigDescriptor;
 
/* USB interface descriptor */
typedef struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
} __attribute__((packed)) UhsInterfaceDescriptor;

/* USB setup packet */
typedef struct
{
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} __attribute__((packed)) UhsSetupPacket;

#endif
