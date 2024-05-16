#ifndef __CYANIDE__GDT_H
#define __CYANIDE__GDT_H

#include "types.h"

namespace cyanide
{
    
    class GlobalDescriptorTable
    {
        public:

            class SegmentDescriptor
            {
                private:
                    cyanide::common::uint16_t limit_lo;
                    cyanide::common::uint16_t base_lo;
                    cyanide::common::uint8_t base_hi;
                    cyanide::common::uint8_t type;
                    cyanide::common::uint8_t limit_hi;
                    cyanide::common::uint8_t base_vhi;

                public:
                    SegmentDescriptor(cyanide::common::uint32_t base, cyanide::common::uint32_t limit, cyanide::common::uint8_t type);
                    cyanide::common::uint32_t Base();
                    cyanide::common::uint32_t Limit();
            } __attribute__((packed));

        private:
            SegmentDescriptor nullSegmentSelector;
            SegmentDescriptor unusedSegmentSelector;
            SegmentDescriptor codeSegmentSelector;
            SegmentDescriptor dataSegmentSelector;

        public:

            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

            cyanide::common::uint16_t CodeSegmentSelector();
            cyanide::common::uint16_t DataSegmentSelector();
    };

}

#endif
