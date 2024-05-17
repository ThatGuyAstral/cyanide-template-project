
#ifndef __CYANIDE__HARDWARE__INTERRUPTS_H
#define __CYANIDE__HARDWARE__INTERRUPTS_H

#include "../gdt.h"
#include "../types.h"
#include "port.h"

namespace cyanide
{
    namespace hardware
    {
        class InterruptManager;

        class InterruptHandler
        {
        protected:
            cyanide::common::uint8_t InterruptNumber;
            InterruptManager* interruptManager;
            InterruptHandler(InterruptManager* interruptManager, cyanide::common::uint8_t InterruptNumber);
            ~InterruptHandler();
        public:
            virtual cyanide::common::uint32_t HandleInterrupt(cyanide::common::uint32_t esp);
        };


        class InterruptManager
        {
            friend class InterruptHandler;
            protected:

                static InterruptManager* ActiveInterruptManager;
                InterruptHandler* handlers[256];

                struct GateDescriptor
                {
                    cyanide::common::uint16_t handlerAddressLowBits;
                    cyanide::common::uint16_t gdt_codeSegmentSelector;
                    cyanide::common::uint8_t reserved;
                    cyanide::common::uint8_t access;
                    cyanide::common::uint16_t handlerAddressHighBits;
                } __attribute__((packed));

                static GateDescriptor interruptDescriptorTable[256];

                struct InterruptDescriptorTablePointer
                {
                    cyanide::common::uint16_t size;
                    cyanide::common::uint32_t base;
                } __attribute__((packed));

                cyanide::common::uint16_t hardwareInterruptOffset;
                static void SetInterruptDescriptorTableEntry(cyanide::common::uint8_t interrupt,
                    cyanide::common::uint16_t codeSegmentSelectorOffset, void (*handler)(),
                    cyanide::common::uint8_t DescriptorPrivilegeLevel, cyanide::common::uint8_t DescriptorType);


                static void InterruptIgnore();

                static void HandleInterruptRequest0x00();
                static void HandleInterruptRequest0x01();
                static void HandleInterruptRequest0x02();
                static void HandleInterruptRequest0x03();
                static void HandleInterruptRequest0x04();
                static void HandleInterruptRequest0x05();
                static void HandleInterruptRequest0x06();
                static void HandleInterruptRequest0x07();
                static void HandleInterruptRequest0x08();
                static void HandleInterruptRequest0x09();
                static void HandleInterruptRequest0x0A();
                static void HandleInterruptRequest0x0B();
                static void HandleInterruptRequest0x0C();
                static void HandleInterruptRequest0x0D();
                static void HandleInterruptRequest0x0E();
                static void HandleInterruptRequest0x0F();
                static void HandleInterruptRequest0x31();

                static void HandleException0x00();
                static void HandleException0x01();
                static void HandleException0x02();
                static void HandleException0x03();
                static void HandleException0x04();
                static void HandleException0x05();
                static void HandleException0x06();
                static void HandleException0x07();
                static void HandleException0x08();
                static void HandleException0x09();
                static void HandleException0x0A();
                static void HandleException0x0B();
                static void HandleException0x0C();
                static void HandleException0x0D();
                static void HandleException0x0E();
                static void HandleException0x0F();
                static void HandleException0x10();
                static void HandleException0x11();
                static void HandleException0x12();
                static void HandleException0x13();

                static cyanide::common::uint32_t HandleInterrupt(cyanide::common::uint8_t interrupt, cyanide::common::uint32_t esp);
                cyanide::common::uint32_t DoHandleInterrupt(cyanide::common::uint8_t interrupt, cyanide::common::uint32_t esp);

                Port8BitSlow programmableInterruptControllerMasterCommandPort;
                Port8BitSlow programmableInterruptControllerMasterDataPort;
                Port8BitSlow programmableInterruptControllerSlaveCommandPort;
                Port8BitSlow programmableInterruptControllerSlaveDataPort;

            public:
                InterruptManager(cyanide::common::uint16_t hardwareInterruptOffset, cyanide::GlobalDescriptorTable* globalDescriptorTable);
                ~InterruptManager();
                cyanide::common::uint16_t HardwareInterruptOffset();
                void Activate();
                void Deactivate();
        };
        
    }
}

#endif