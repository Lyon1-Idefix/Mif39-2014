#include "QImageLoader/qimageloader.hpp"
#include "Wavefront/wavefront.hpp"

#include <iostream>
#include <fstream>

extern void __attach(void);
extern void __attachInterfaces(void);
extern void __attachGenerics(void);
extern void __attachAssets(void);
extern void __attachQImage(void);
extern void __attachWavefront(void);

QString dump ( Assets::SpawnPoint* spawn, int indent = 0 );
QString dump ( Assets::GameEntityElement* spawn, int indent = 0 );

QString indenter ( int indent = 0 )
{
    QString result = "";
    for ( int i = 0 ; i < indent ; i ++ )
        result += "  ";
    return result;
}

template < class Type > QString toString ( Type v, int indent = 0 ) {
    QString result = "";
    result += v;
    return result;
}

template <> QString toString <float> ( float v, int indent ) {
    QString result = "";
    result += "float(";
    result += QString::number(v);
    result += ")";
    return result;
}

template <> QString toString <QUuid> ( QUuid v, int indent ) {
    QString result = "";
    result += "QUuid(" + v.toString() +")";
    return result;
}

template <> QString toString <Vector3d> ( Vector3d v, int indent ) {
    QString result = "";
    result += "Vector3d(";
    result += QString::number(v.x);
    result += " ";
    result += QString::number(v.y);
    result += " ";
    result += QString::number(v.z);
    result += ")";
    return result;
}

template <> QString toString <Vector4d> ( Vector4d v, int indent ) {
    QString result = "";
    result += "Vector3d(";
    result += QString::number(v.x);
    result += " ";
    result += QString::number(v.y);
    result += " ";
    result += QString::number(v.z);
    result += " ";
    result += QString::number(v.w);
    result += ")";
    return result;
}

template <> QString toString <Vector2ui> ( Vector2ui v, int indent ) {
    QString result = "";
    result += "Vector2ui(";
    result += QString::number(v.w);
    result += " ";
    result += QString::number(v.h);
    result += ")";
    return result;
}

template <> QString toString <Assets::SpawnPoint*> ( Assets::SpawnPoint* v, int indent ) {
    QString result = "";
    result += dump ( v, indent );
    return result;
}

template <> QString toString <Assets::GameEntityElement*> ( Assets::GameEntityElement* v, int indent ) {
    QString result = "";
    result += dump ( v, indent );
    return result;
}

template < class AttributeType > QString dump ( AttributeContainer* container, QString name, int indent = 0 ) {
    QString result = "";
    result += indenter ( indent ) + name + " = [" + toString ( container->get < AttributeType > ( name ), indent + 1 ) + "]\n";
    return result;
}

template < class AttributeType > QString dumpIndexed ( AttributeContainer* container, QString name, int indent = 0 ) {
    QString result = "";
    result += indenter ( indent ) + name + " = [\n";
    for ( int i = 0 ; i < container->getSize < AttributeType > ( name ) ; i ++ )
        result += indenter ( indent + 1 ) + " - " + toString < AttributeType > ( container->get < AttributeType > ( name, i ), indent + 2 ) + "\n";
    result += indenter ( indent ) + "]\n";
    return result;
}

QString dump ( Assets::SpawnPoint* spawn, int indent )
{
    QString result = "";
    result += indenter ( indent ) + spawn->getName() + "\n";
    result += dump < QString > ( spawn, "_RealName", indent + 1 );
    result += dump < QUuid > ( spawn, "World", indent + 1 );
    result += dump < Vector3d > ( spawn, "Location", indent + 1 );
    result += dump < float > ( spawn, "Size", indent + 1 );
    result += dumpIndexed < QString > ( spawn, "Semantics", indent + 1 );
    return result;
}

QString dump ( Assets::GameEntityElement* elt, int indent )
{
    QString result = "";
    result += indenter ( indent ) + elt->getName() + "\n";
    //result += dump < QString > ( elt, "_RealName", indent + 1 );
    result += dump < QUuid > ( elt, "Entity", indent + 1 );
    result += dump < Vector3d > ( elt, "Position", indent + 1 );
    result += dump < Vector4d > ( elt, "Rotation", indent + 1 );
    result += dump < Vector3d > ( elt, "Scale", indent + 1 );
    result += dumpIndexed < QString > ( elt, "Semantics", indent + 1 );
    result += dumpIndexed < QUuid > ( elt, "Resources", indent + 1 );
    return result;
}

QString dump ( Assets::WorldPtr world, int indent = 0 )
{
    QString result = "";
    result += indenter ( indent ) + world->getName() + " " + world->getUUID().toString () + "\n";
    result += dump < QString > ( world.data(), "_RealName", indent + 1 );
    result += dump < Vector3d > ( world.data(), "Extents", indent + 1 );
    result += dump < Vector2ui > ( world.data(), "Subdivision", indent + 1 );
    result += dumpIndexed < Assets::SpawnPoint* > ( world.data(), "SpawnPoints", indent + 1 );
    result += dumpIndexed < QString > ( world.data(), "Semantics", indent + 1 );
    result += dumpIndexed < QUuid > ( world.data(), "Cells", indent + 1 );
    return result;
}

QString dump ( Assets::WorldCellPtr cell, int indent = 0 )
{
    QString result = "";
    result += indenter ( indent ) + cell->getName() + " " + cell->getUUID().toString () + "\n";
    result += dump < QString > ( cell.data(), "_RealName", indent + 1 );
    result += dump < QUuid > ( cell.data(), "World", indent + 1 );
    result += dump < Vector2ui > ( cell.data(), "Indices", indent + 1 );
    result += dump < Vector3d > ( cell.data(), "Extents", indent + 1 );
    result += dump < Vector3d > ( cell.data(), "Position", indent + 1 );
    result += dumpIndexed < QUuid > ( cell.data(), "GameObjects", indent + 1 );
    return result;
}

QString dump ( Assets::GameObjectPtr cell, int indent = 0 )
{
    QString result = "";
    result += indenter ( indent ) + cell->getName() + " " + cell->getUUID().toString () + "\n";
    result += dump < QString > ( cell.data(), "_RealName", indent + 1 );
    result += dump < QUuid > ( cell.data(), "Cell", indent + 1 );
    result += dump < Vector3d > ( cell.data(), "Position", indent + 1 );
    result += dump < Vector4d > ( cell.data(), "Rotation", indent + 1 );
    result += dump < Vector3d > ( cell.data(), "Scale", indent + 1 );
    result += dump < QUuid > ( cell.data(), "Entity", indent + 1 );
    result += dumpIndexed < QString > ( cell.data(), "Semantics", indent + 1 );
    return result;
}

QString dump ( Assets::GameEntityPtr cell, int indent = 0 )
{
    QString result = "";
    result += indenter ( indent ) + cell->getName() + " " + cell->getUUID().toString () + "\n";
    result += dump < QString > ( cell.data(), "_RealName", indent + 1 );
    result += dumpIndexed < QString > ( cell.data(), "Semantics", indent + 1 );
    result += dumpIndexed < Assets::GameEntityElement* > ( cell.data(), "Elements", indent + 1 );
    return result;
}

#define LOGME std::cerr << __FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl

int main(int argc, char *argv[])
{
    /*
    __attach();
    __attachInterfaces();
    __attachGenerics();
    __attachAssets();*/
    __attachQImage();
    __attachWavefront();

    unsigned long long index = 0;
    FileDescriptor file ( argv[1]);
    QList < SharedResourcePtr > ress = ResourceHolder::Load(file);
    Assets::WorldPtr world = ress[0].dynamicCast < Assets::World > ();
    ByteBuffer buffer = ResourceHolder::ToBuffer(world);
    SharedResourcePtr res = ResourceHolder::FromBuffer(buffer,index);
    Assets::WorldPtr nworld = res.dynamicCast < Assets::World > ();

    std::ofstream out1;
    std::ofstream out2;
    out1.open ( "input.txt" );
    out2.open ( "output.txt" );

    out1 << dump ( world ).toStdString();
    out2 << dump ( nworld ).toStdString();
    for ( int i = 0 ; i < world->getSize < QUuid > ( "Cells" ) ; i ++ ) {
        index = 0;
        QUuid uuid = world->get < QUuid > ( "Cells", i );
        Assets::WorldCellPtr cell = ResourceHolder::GetByUUID(uuid).dynamicCast < Assets::WorldCell > ();
        ByteBuffer cbuffer = ResourceHolder::ToBuffer(cell);
        SharedResourcePtr cres = ResourceHolder::FromBuffer(cbuffer,index);
        Assets::WorldCellPtr ncell = cres.dynamicCast < Assets::WorldCell > ();
        out1 << dump ( cell ).toStdString();
        out2 << dump ( ncell ).toStdString();
        for ( int j = 0 ; j < cell->getSize < QUuid > ( "GameObjects" ) ; j ++ ) {
            index = 0;
            QUuid uuid = cell->get < QUuid > ( "GameObjects", j );
            Assets::GameObjectPtr go = ResourceHolder::GetByUUID(uuid).dynamicCast < Assets::GameObject > ();
            ByteBuffer cgbuffer = ResourceHolder::ToBuffer(go);
            SharedResourcePtr gres = ResourceHolder::FromBuffer(cgbuffer,index);
            Assets::GameObjectPtr ngo = gres.dynamicCast < Assets::GameObject > ();
            out1 << dump ( go ).toStdString();
            out2 << dump ( ngo ).toStdString();
            {
                index = 0;
                QUuid uuid = go->get < QUuid > ( "Entity" );
                Assets::GameEntityPtr ge = ResourceHolder::GetByUUID(uuid).dynamicCast < Assets::GameEntity > ();
                ByteBuffer gebuffer = ResourceHolder::ToBuffer(ge);
                SharedResourcePtr geres = ResourceHolder::FromBuffer(gebuffer,index);
                Assets::GameEntityPtr nge = geres.dynamicCast < Assets::GameEntity > ();
                out1 << dump ( ge ).toStdString();
                out2 << dump ( nge ).toStdString();
            }
        }
    }
    out2.close ();
    out1.close ();
    /*
    std::cout << "Create resource" << std::endl;
    SharedResourcePtr ptr = ResourceHolder::CreateByName( "World", "Adventure world" );
    ptr->set < QString > ( "_RealName", "toto" );
    ptr->add < QString > ( "Semantics", "World" ); ptr->add < QString > ( "Semantics", "Reality" );
    ptr->add < QUuid > ( "Cells", ptr->getUUID() );
    ptr->add < Assets::SpawnPoint* > ( "SpawnPoints", new Assets::SpawnPoint ( ptr.dynamicCast < Assets::World > (), "spawn" ) );
    std::cout << dump ( ptr.dynamicCast < Assets::World > () ).toStdString() << std::endl;

    std::cout << "Convert to buffer" << std::endl;
    ByteBuffer buffer = ResourceHolder::ToBuffer(ptr);

    std::cout << "Convert from buffer" << std::endl;
    SharedResourcePtr res = ResourceHolder::FromBuffer(buffer,index);
    std::cout << dump ( ptr.dynamicCast < Assets::World > () ).toStdString() << std::endl;
    */

    /*
    for ( int i = 1 ; i < argc ; i ++ ) {
        FileDescriptor file ( argv[i]);
        QList < SharedResourcePtr > ress = ResourceHolder::Load(file);
    }

    foreach ( QUuid id, ResourceHolder::AllKeys() ) {
        SharedResourcePtr res = ResourceHolder::GetByUUID(id);
        std::cout << id.toString().toStdString() << " :: " << res->getName().toStdString();
        if ( res->getAttr("_RealName") != NULL )
            std::cout << " :: " << res->get < QString > ( "_RealName" ).toStdString();
        std::cout << std::endl;
        ByteBuffer buffer = res->convertToBuffer();
        FILE* fd = fopen ( ("Bins/" + ResourceHolder::GetTypeByUUID (res->mTypeID) + "-" + id.toString() + ".binary" ).toStdString().c_str(), "wb" );
        fwrite ( buffer.getData(), 1, buffer.getLength(), fd );
        fclose ( fd );
    }
    */
    return 0;
}
