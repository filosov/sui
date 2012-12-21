#include "scstorage.hpp"
extern "C"
{
#include "sc_memory_headers.h"
}
#include "util.hpp"
#include "scstoragestream.hpp"

ScStorage::ScStorage(QObject *parent)
{
}

ScStorage::~ScStorage()
{
}

void ScStorage::_printEl(const ScUri &uri)
{

}

bool ScStorage::initialize(const QString &path)
{
    return sc_memory_initialize(path.toAscii().data());
}

void ScStorage::shutdown()
{
    sc_memory_shutdown();
}

ScUri ScStorage::gen_el(const ScElementType &type)
{
    ScUri uri = ScUri();
    if (!type.isArcOrEdge())
    {
        uri = sc_addr2ScUri(sc_memory_node_new(ScElementType2sc_type(type)));
    }
    return uri;
}

bool ScStorage::erase_el(const ScUri &uri)
{
    QIODevice *dev = mMapUri2Device.value(uri);
    if (dev)
        dev->close();
    return sc_memory_element_free(ScUri2sc_addr(uri));
}

bool ScStorage::set_beg(const ScUri &uri, const ScUri &beg_el)
{
    return false;
}

ScUri ScStorage::get_beg(const ScUri &uri)
{
    sc_addr resAddr;
    sc_memory_get_arc_begin(ScUri2sc_addr(uri), &resAddr);
    return sc_addr2ScUri(resAddr);
}

bool ScStorage::set_end(const ScUri &uri, const ScUri &end_el)
{
    return false;
}

ScUri ScStorage::get_end(const ScUri &uri)
{
    sc_addr resAddr;
    sc_memory_get_arc_end(ScUri2sc_addr(uri), &resAddr);
    return sc_addr2ScUri(resAddr);
}

bool ScStorage::set_content(const ScUri &uri, QIODevice &device)
{
    ScStorageStream *stream = qobject_cast<ScStorageStream*>(&device);
    if (!stream)
    {
        stream = new ScStorageStream(&device);
        mMapUri2Device[uri] = stream;
    }
    return sc_memory_set_link_content(ScUri2sc_addr(uri), stream->scStream()) == SC_OK ? true : false;
}

QIODevice *ScStorage::open_content(const ScUri &uri)
{
    ScStorageStream *device = new ScStorageStream;
    _sc_stream *stream;
    sc_memory_get_link_content(ScUri2sc_addr(uri), &stream);
    device->setStream(stream);
    mMapUri2Device[uri] = device;
    return device;
}

bool ScStorage::close_content(QIODevice *device)
{
    ScStorageStream *stream = qobject_cast<ScStorageStream*>(device);
    if (stream)
        return sc_stream_free(stream->scStream()) == SC_OK ? true : false;
    return false;
}

bool ScStorage::is_content_opened(const ScUri &uri) const
{
    ScStorageStream *dev = mMapUri2Device.value(uri);
    if (dev && dev->isOpen())
        return true;
    return false;
}

bool ScStorage::find_content(QIODevice &device, ScUriList &result)
{
    result.clear();
    ScStorageStream *stream = qobject_cast<ScStorageStream*>(&device);
    if (!stream)
        return false;
    sc_addr *tempRes = 0;
    sc_uint32 resSize = 0;
    sc_result flag = sc_memory_find_links_with_content(stream->scStream(), &tempRes, &resSize);
    if (flag != SC_OK)
        return false;
    for(int i = 0; i < resSize; ++i)
        result.append(sc_addr2ScUri(*tempRes++));
    return true;
}

bool ScStorage::find_str_content(const QString &str, ScUriList &result)
{
    QBuffer buffer;
    QByteArray data = str.toUtf8();
    data.append((char)0);
    buffer.open(QIODevice::WriteOnly);
    buffer.write(data.data(), data.size());
    buffer.close();

    return find_content(buffer, result);
}

QString ScStorage::decode_str_content(QIODevice &device)
{
    device.open(QIODevice::ReadOnly);
    QByteArray data = device.readAll();
    QString res = QString::fromUtf8(data.data(), data.size() - 1);  // don't use last byte, we have added it manualy
    device.close();
    return res;
}

bool ScStorage::set_el_type(const ScUri &uri, const ScElementType &type)
{
    return false;
}

ScElementType ScStorage::get_el_type(const ScUri &uri) const
{
    sc_type type;
    sc_memory_get_element_type(ScUri2sc_addr(uri), &type);
    return sc_type2ScElementType(type);
}

ScIteratorInterface *ScStorage::create_iterator(const ScTemplate &templ)
{
    return 0;
}

bool ScStorage::destroy_iterator(ScIteratorInterface *it)
{
    return false;
}

bool ScStorage::connect_callback(const ScEventType &eventType, const ScUri &uri, ScEventCallbackInterface *callback)
{
    return false;
}

bool ScStorage::disconnect_callback(const ScEventType &eventType, const ScUri &uri, ScEventCallbackInterface *callback)
{
    return false;
}

bool ScStorage::disconnect_callback(ScEventCallbackInterface *callback)
{
    return false;
}
