#ifndef UTIL_HPP
#define UTIL_HPP

#include <QMap>
#include "interfaces/ScMemory.h"
#include "defines.hpp"
extern "C"
{
#include "sc-store/sc_types.h"
#include "sc-store/sc_stream_private.h"
}

class SCSCTORESHARED_EXPORT ScTypesMapper
{
public:
    static ScTypesMapper* instance();
    static void destroyInstance();
    ScElementType sc_type2ScElementTypeConv(const sc_type type) const;
    sc_type ScElementType2sc_typeConv(const ScElementType& type) const;
private:
    static ScTypesMapper *mInstance;
    QMap<quint16, quint16> mTypeMappings;
    explicit ScTypesMapper();
    virtual ~ScTypesMapper();
};

#define ScElementType2sc_type ScTypesMapper::instance()->ScElementType2sc_typeConv
#define sc_type2ScElementType ScTypesMapper::instance()->sc_type2ScElementTypeConv

class SCSCTORESHARED_EXPORT ScEventTypesMapper
{
public :
    static ScEventTypesMapper* instance();
    static void destroyInstance();
    ScEventType sc_event_type2ScEventTypeConv(const sc_event_type type) const;
    sc_event_type ScEventType2sc_event_typeConv(const ScEventType type) const;
private:
    explicit ScEventTypesMapper();
    virtual ~ScEventTypesMapper();
    static ScEventTypesMapper* mInstance;
    QMap<int, int> mEventsTypeMappings;

};

#define sc_event_type2ScEventType ScEventTypesMapper::instance()->sc_event_type2ScEventTypeConv
#define ScEventType2sc_event_type ScEventTypesMapper::instance()->ScEventType2sc_event_typeConv

#define SEG_ADDR(x) ((x.value()) >> 16) & 0xfff
#define OFFSET_ADDR(x) (x.value()) & 0xfff

#if USE_NETWORK_SCALE
#define NT_SCALE(x) ((##x.value()) >> 32)
SCSCTORESHARED_EXPORT inline ScUri sc_addr2ScUriNTSc(sc_addr addr)
{
    return ScUri(0x0000000000000000 | ((0x0000000000000000 | addr.net_addr) << 32) | ((0x0000000000000000 | addr.seg) << 16) | (addr.offset));
}

SCSCTORESHARED_EXPORT inline sc_addr ScUri2sc_addrNTSc(SCUri uri)
{
    sc_addr addr;
    addr.net_addr = NT_SCALE(uri);
    addr.seg = SEG_ADDR(uri);
    addr.offset = OFFSET_ADDR(uri);
    return addr;
}
#else
    #define NT_SCALE(x)
SCSCTORESHARED_EXPORT inline ScUri sc_addr2ScUriLoc(sc_addr addr)
{
    return ScUri(0x0000000000000000 | ((0x0000000000000000 | addr.seg) << 16) | (addr.offset));
}

SCSCTORESHARED_EXPORT inline sc_addr ScUri2sc_addrLoc(ScUri uri)
{
    sc_addr addr;
    addr.seg = SEG_ADDR(uri);
    addr.offset = OFFSET_ADDR(uri);
    return addr;
}
#endif

#if USE_NETWORK_SCALE
    #define ScUri2sc_addr ScUri2sc_addrNtSc
    #define sc_addr2ScUri sc_addr2ScUriNtSc
#else
    #define ScUri2sc_addr ScUri2sc_addrLoc
    #define sc_addr2ScUri sc_addr2ScUriLoc
#endif

SCSCTORESHARED_EXPORT _sc_stream* IODevice2sc_stream(QIODevice *device);


#endif // UTIL_HPP
