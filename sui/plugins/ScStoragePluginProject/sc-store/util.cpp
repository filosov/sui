#include "util.hpp"
extern "C"{
#include <sc-store/sc_stream_memory.h>
}

#define ScTypeElementMask (ScNode | ScLink | ScEdgeCommon | ScArcCommon)

ScTypesMapper* ScTypesMapper::mInstance = 0;

ScTypesMapper* ScTypesMapper::instance()
{
    if (!mInstance)
        mInstance = new ScTypesMapper;
    return mInstance;
}

void ScTypesMapper::destroyInstance()
{
    if (mInstance)
    {
        delete mInstance;
        mInstance = 0;
    }
}

ScTypesMapper::ScTypesMapper()
{
    mTypeMappings[sc_type_node] = ScNode;
    mTypeMappings[sc_type_link] = ScLink;
    mTypeMappings[sc_type_edge_common] = ScEdgeCommon;
    mTypeMappings[sc_type_arc_common] = ScArcCommon;
    mTypeMappings[sc_type_arc_access] = ScArcMain;
    mTypeMappings[sc_type_const] = ScConst;
    mTypeMappings[sc_type_var] = ScVar;
    mTypeMappings[sc_type_arc_pos] = ScPos;
    mTypeMappings[sc_type_arc_neg] = ScNeg;
    mTypeMappings[sc_type_arc_fuz] = ScFuz;
    mTypeMappings[sc_type_arc_temp] = ScTemp;
    mTypeMappings[sc_type_arc_perm] = ScPerm;
    mTypeMappings[sc_type_node_tuple] = ScTuple;
    mTypeMappings[sc_type_node_struct] = ScStruct;
    mTypeMappings[sc_type_node_role] = ScRole;
    mTypeMappings[sc_type_node_norole] = ScBinary;
    mTypeMappings[sc_type_node_class] = ScConcept;
    mTypeMappings[sc_type_node_abstract] = ScAbstract;
    //! TODO:
    mTypeMappings[sc_type_node_material] = 0;
    //#define sc_type_arc_pos_const_perm (sc_type_arc_access | sc_type_const | sc_type_arc_pos | sc_type_arc_perm)
    //#define sc_type_element_mask     (sc_type_node | sc_type_link | sc_type_edge_common | sc_type_arc_common | sc_type_arc_access)
    //#define ScAccessory         8192

    mTypeMappings[sc_type_permanency_mask] = ScPermMask;
    mTypeMappings[sc_type_arc_mask] = ScArcMask;
    mTypeMappings[sc_type_node_struct_mask] = ScStructMask;
    mTypeMappings[sc_type_constancy_mask] = ScConstMask;
    mTypeMappings[sc_type_positivity_mask] = ScPosMask;
    mTypeMappings[sc_type_element_mask] = ScTypeElementMask;
//    mTypeMappings[] = ;
}

ScTypesMapper::~ScTypesMapper()
{
    mTypeMappings.clear();
}

sc_type ScTypesMapper::ScElementType2sc_typeConv(const ScElementType &type) const
{
    sc_type outputType = 0;
    outputType |= mTypeMappings.key(type.value() & ScTypeElementMask, 0);
    if (type.isArcOrEdge())
    {
        outputType |= mTypeMappings.key(type.value() & ScArcMask, 0);
        outputType |= mTypeMappings.key(type.value() & ScPosMask, 0);
        outputType |= mTypeMappings.key(type.value() & ScPermMask, 0);
    }
    else
    {
        outputType |= mTypeMappings.key(type.value() & ScStructMask, 0);
    }
    outputType |= mTypeMappings.key(type.value() & ScConstMask, 0);
    return outputType;
}

ScElementType ScTypesMapper::sc_type2ScElementTypeConv(const sc_type type) const
{
    ScElementType outputType;
    outputType.add(mTypeMappings.value(type & sc_type_element_mask, 0));
    if ((type & sc_type_element_mask) & sc_type_arc_mask)
    {
        outputType.add(mTypeMappings.value(type & sc_type_permanency_mask));
        outputType.add(mTypeMappings.value(type & sc_type_positivity_mask));
        outputType.add(mTypeMappings.value(type & sc_type_arc_mask));
    }
    else
    {
        outputType.add(mTypeMappings.value(type & sc_type_node_struct_mask));
    }
    outputType.add(mTypeMappings.value(type & sc_type_constancy_mask));
    return outputType;
}

ScEventTypesMapper* ScEventTypesMapper::mInstance = 0;

ScEventTypesMapper *ScEventTypesMapper::instance()
{
    if (!mInstance)
        mInstance = new ScEventTypesMapper;
    return mInstance;
}

void ScEventTypesMapper::destroyInstance()
{
    if (mInstance)
    {
        delete mInstance;
        mInstance = 0;
    }
}

ScEventType ScEventTypesMapper::sc_event_type2ScEventTypeConv(const sc_event_type type) const
{
    ScEventType resultType = static_cast<ScEventType>(mEventsTypeMappings.value(type, ScEventUnknown));
    return resultType;
}

sc_event_type ScEventTypesMapper::ScEventType2sc_event_typeConv(const ScEventType type) const
{
    //Temporary. Need to be updated after defition unknown event type in sc-storage
    Q_ASSERT_X(mEventsTypeMappings.key(type, 28500) != 28500,
               "ScEventTypeMapper", "ScEventType cannot be translated into sc_event_type");
    return static_cast<sc_event_type>(mEventsTypeMappings.key(type));
}

ScEventTypesMapper::ScEventTypesMapper()
{
    mEventsTypeMappings[SC_EVENT_ADD_OUTPUT_ARC] = ScEventOutputEdgeAdded;
    mEventsTypeMappings[SC_EVENT_ADD_INPUT_ARC] = ScEventOutputEdgeRemoved;
    mEventsTypeMappings[SC_EVENT_REMOVE_OUTPUT_ARC] = ScEventOutputEdgeRemoved;
    mEventsTypeMappings[SC_EVENT_REMOVE_INPUT_ARC] = ScEventInputEdgeRemoved;
    mEventsTypeMappings[SC_EVENT_CHANGE_LINK_CONTENT] = ScEventContentChanged;
}

ScEventTypesMapper::~ScEventTypesMapper()
{
    mEventsTypeMappings.clear();
}


_sc_stream* IODevice2sc_stream(QIODevice *device)
{
    if (!device->open(QIODevice::ReadOnly))
        return 0;
    QByteArray buffer = device->readAll();
    _sc_stream *stream = sc_stream_memory_new(buffer.data(), buffer.size(), SC_STREAM_READ, SC_FALSE);
    device->close();
    return stream;
}
