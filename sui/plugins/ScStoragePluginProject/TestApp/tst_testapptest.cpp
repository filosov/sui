#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QPluginLoader>
#include <QBuffer>
#include <interfaces/ScMemory.h>
#include <interfaces/UiPlugins.h>
#include <sc_memory_headers.h>
#include <util.hpp>
#include <scstorage.hpp>
#include <scstoragestream.hpp>

Q_DECLARE_METATYPE(ScElementType)

class TestAppTest : public QObject
{
    Q_OBJECT
    
public:
    explicit TestAppTest();
    ~TestAppTest();
private Q_SLOTS:
    void testElementTypeConvertion_data();

    void testElementTypeConvertion();
    void testLoading();
    void testPluginInterface();
    void testUriAddrConvertion();

    void testAddressConvertion();

    void testGeneration();
    void testContent();
private:
    QPluginLoader *mLoader;
    QString mLoadErrorString;
};

TestAppTest::TestAppTest()
{
    QString fileName = "sc-store.dll";
    mLoader = new QPluginLoader();
    mLoader->setFileName(fileName);
    if(!mLoader->load())
    {
        mLoadErrorString = mLoader->errorString();
    }
}

TestAppTest::~TestAppTest()
{
    mLoader->unload();
    delete mLoader;
}

void TestAppTest::testElementTypeConvertion_data()
{
    QTest::addColumn<int>("sc_types");
    QTest::addColumn<ScElementType>("ScTypes");
    QTest::newRow("test node type") << sc_type_node << ScElementType(ScNode);
    QTest::newRow("test arc common type") << sc_type_arc_common << ScElementType(ScArcCommon);
    QTest::newRow("test node norole") << (sc_type_node | sc_type_node_norole) << ScElementType(ScNode | ScBinary);
    QTest::newRow("test arc ac, neg, const, perm")
            << (sc_type_arc_access | sc_type_arc_neg | sc_type_arc_perm | sc_type_const)
            << ScElementType(ScArcCommon | ScNeg | ScConst | ScPerm);
}

void TestAppTest::testLoading()
{
    QVERIFY2(mLoader->isLoaded(), mLoadErrorString.toAscii());
}

void TestAppTest::testPluginInterface()
{
    UiPluginInterface *interface = qobject_cast<UiPluginInterface*>(mLoader->instance());
    QVERIFY2(interface, "No compabiblity interface");
}

void TestAppTest::testUriAddrConvertion()
{
}

void TestAppTest::testAddressConvertion()
{
}

void TestAppTest::testGeneration()
{
    UiPluginInterface *interface = qobject_cast<UiPluginInterface*>(mLoader->instance());
    qDebug()<<"Interface name: "<<interface->name();
    qDebug()<<"Interface version: "<<interface->version();
    interface->initialize((UiRootInterface*)(0x00003451));
    ScMemoryFactoryInterface *factory = qobject_cast<ScMemoryFactoryInterface*>(interface->interfaces().at(0));
    ScMemoryInterface * memory = factory->createInstance();
    memory->initialize("./");
    ScUri uri = memory->gen_el(ScElementType(ScNode | ScRole | ScConst));
    QVERIFY2(uri != ScUriEmpty, "Generation failed");
    QVERIFY2(memory->erase_el(uri), "Erasing failed");
    memory->shutdown();
}

void TestAppTest::testContent()
{
    UiPluginInterface *interface = qobject_cast<UiPluginInterface*>(mLoader->instance());
    interface->initialize((UiRootInterface*)(0x00003451));
    ScMemoryFactoryInterface *factory = qobject_cast<ScMemoryFactoryInterface*>(interface->interfaces().at(0));
    ScMemoryInterface * memory = factory->createInstance();
    memory->initialize("./");
    QList<ScUri> uriList;
    QList<ScUri> uriList2;
    for (int i = 0; i < 10 ; ++i)
    {
        ScUri uri = memory->gen_el(ScElementType(ScLink));
        QBuffer buf;
        buf.open(QIODevice::WriteOnly);
        QByteArray data;
        data.append("Content: ");
        data.append(QString::number(i % 3));
        buf.write(data);
        QVERIFY2(memory->set_content(uri, buf), "Cannot set content");
        uriList.append(uri);
        if (i % 3 == 2)
            uriList2.append(uri);
        buf.close();
    }
    QBuffer testBuf;
    testBuf.open(QIODevice::ReadWrite);
    QByteArray d;
    d.append("Content: 2");
    testBuf.write(d);
    QList<ScUri> foundLink;
    ScStorageStream *st = new ScStorageStream(&testBuf);
    memory->find_content(*st, foundLink);
    delete st;
    testBuf.close();
    qDebug()<<"Links founded: "<<foundLink.size();
    QVERIFY2(foundLink == uriList2, "Error finding content");
    memory->shutdown();
}

void TestAppTest::testElementTypeConvertion()
{
    QFETCH(int, sc_types);
    QFETCH(ScElementType, ScTypes);
    QVERIFY2(sc_type2ScElementType(sc_types) == ScTypes, "Failure");

}

QTEST_APPLESS_MAIN(TestAppTest)

#include "moc/tst_testapptest.moc"
