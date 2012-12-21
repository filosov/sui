#ifndef SCSTORAGE_HPP
#define SCSTORAGE_HPP

#include <QObject>
#include "defines.hpp"
#include "interfaces/ScMemory.h"

class ScStorageStream;

/**
* @class ScStorage
* @brief Class realized memory interface and wrapped logic of sc-store
*/
class SCSCTORESHARED_EXPORT ScStorage : public QObject, public ScMemoryInterface
{
    Q_OBJECT
    Q_INTERFACES(ScMemoryInterface)
public:
    //! Default constructor
    explicit ScStorage(QObject *parent = 0);
    /**
    * @brief Default destructor
    * @note Before deletion of instance call shutdown() for successful termination
    */
    virtual ~ScStorage();

    /**
    * @brief Print debug information about sc element
    * @param uri Uri of sc element
    * @note Do nothing, because there is no implementation for printing debug information in sc-store
    */
    void _printEl(const ScUri &uri);

    //! @copydoc ScMemoryInterface::initialize
    bool initialize(const QString &path);

    //! @copydoc ScMemoryInterface::shutdown
    void shutdown();

    /**
    * @copydoc ScMemoryInterface::gen_el
    * @note In current version generate <b>only</b> node
    * @todo Split interfaces for node and arc generations
    */
    ScUri gen_el(const ScElementType &type);

    /**
     * @copydoc ScMemoryInterface::erase_el
     * @note Close io device assosiated with current element
     */
    bool erase_el(const ScUri &uri);

    /**
    * @copydoc ScMemoryInterface::set_beg
    * @note Do nothing. Always returned false
    */
    bool set_beg(const ScUri &uri, const ScUri &beg_el);

    /**
    * @copydoc ScMemoryInterface::get_beg
    * @note Do nothing. Always returned false
    */
    ScUri get_beg(const ScUri &uri);

    /**
    * @copydoc ScMemoryInterface::get_beg
    * @note Do nothing. Always returned false
    */
    bool set_end(const ScUri &uri, const ScUri &end_el);

    /**
    * @copydoc ScMemoryInterface::get_beg
    * @note Do nothing. Always returned false
    */
    ScUri get_end(const ScUri &uri);

    //! @copydoc ScMemoryInterface::set_content
    bool set_content(const ScUri &uri, QIODevice &device);

    //! @copydoc ScMemoryInterface::open_content
    QIODevice* open_content(const ScUri &uri);

    //! @copydoc ScMemoryInterface::close_content
    bool close_content(QIODevice *device);

    //! @copydoc ScMemoryInterface::is_content_opened
    bool is_content_opened(const ScUri &uri) const;

    //! @copydoc ScMemoryInterface::find_content
    bool find_content(QIODevice &device, ScUriList &result);

    //! @copydoc ScMemoryInterface::find_str_content
    bool find_str_content(const QString &str, ScUriList &result);

    //! @copydoc ScMemoryInterface::decode_str_content
    QString decode_str_content(QIODevice &device);

    //! @copydoc ScMemoryInterface::set_el_type
    bool set_el_type(const ScUri &uri, const ScElementType &type);

    //! @copydoc ScMemoryInterface::get_el_type
    ScElementType get_el_type(const ScUri &uri) const;

    //! @copydoc ScMemoryInterface::create_iterator
    ScIteratorInterface* create_iterator(const ScTemplate &templ);

    //! @copydoc ScMemoryInterface::destroy_iterator
    bool destroy_iterator(ScIteratorInterface *it);

    //! @copydoc ScMemoryInterface::connect_callback
    bool connect_callback(const ScEventType &eventType,
                                  const ScUri &uri,
                                  ScEventCallbackInterface *callback);

    //! @copydoc ScMemoryInterface::disconnect_callback
    bool disconnect_callback(const ScEventType &eventType,
                                     const ScUri &uri,
                                     ScEventCallbackInterface *callback);

    //! @copydoc ScMemoryInterface::disconnect_callback
    bool disconnect_callback(ScEventCallbackInterface *callback);

private:
    //! Mapping between element's uri and stream object for working with theirs contents
    QMap<ScUri, ScStorageStream*> mMapUri2Device;

};

class SCSCTORESHARED_EXPORT ScStorageFactory : public QObject, public ScMemoryFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(ScMemoryFactoryInterface)
public:
    //! Default constructor
    explicit ScStorageFactory(QObject *parent = 0) :
        QObject(parent)
    {
    }
    //! Default desctructor
    virtual ~ScStorageFactory() { }

    //! Create instance of sc storage
    ScMemoryInterface* createInstance()
    {
        return new ScStorage(this);
    }
};

#endif // SCSTORAGE_HPP
