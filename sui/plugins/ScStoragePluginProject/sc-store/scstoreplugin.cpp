#include "scstoreplugin.hpp"
#include "scstorage.hpp"
#include "util.hpp"

Q_EXPORT_PLUGIN2(sc-store, ScStorePlugin)

ScStorePlugin::ScStorePlugin(QObject *parent) :
    QObject(parent),
    mPluginName("sc-store"),
    mPluginVersion("0.0.1")
{
}

ScStorePlugin::~ScStorePlugin()
{

}

const QString& ScStorePlugin::name() const
{
    return mPluginName;
}

const QString& ScStorePlugin::version() const
{
    return mPluginVersion;
}

const QList<QObject*>& ScStorePlugin::interfaces() const
{
    return mPluginInterfaces;
}

void ScStorePlugin::initialize(UiRootInterface *root)
{
    Q_ASSERT_X(root != 0, "ScStorePlugin::initialize", "Null pointer to the root interface");
    mPluginInterfaces.push_back(new ScStorageFactory(this));
}

void ScStorePlugin::shutdown()
{
    ScTypesMapper::destroyInstance();
    ScEventTypesMapper::destroyInstance();
}

