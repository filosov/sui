#ifndef SCSTOREPLUGIN_HPP
#define SCSTOREPLUGIN_HPP

#include <QObject>
#include "defines.hpp"
#include "interfaces/UiPlugins.h"

class SCSCTORESHARED_EXPORT ScStorePlugin : public QObject, public UiPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(UiPluginInterface)
public:
    explicit ScStorePlugin(QObject *parent = 0);
    virtual ~ScStorePlugin();
    //! Returns plugin name
    const QString& name() const;
    //! Returns plugin version
    const QString& version() const;
    /*! Return list of all interfaces that realized in that plugin
      * @attention All interfaces need to be a QObject, and created with plugin interface as a parent object
      */
    const QList<QObject*>& interfaces() const;
    /*! Initialize plugin
      * @param root Pointer to root object
      */
    void initialize(UiRootInterface *root);

    //! Shutdown plugin
    void shutdown();

protected:
    QString mPluginName;
    QString mPluginVersion;
    QList<QObject*> mPluginInterfaces;
};

#endif // SCSTOREPLUGIN_HPP
