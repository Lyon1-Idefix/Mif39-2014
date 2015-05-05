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

#if 1
    FileDescriptor file ( argv[1]);
    QList < SharedResourcePtr > ress = ResourceHolder::Load(file);
    std::cout << "World: " << ResourceHolder::GetAllByTypeName("World" ).size() << std::endl;
    std::cout << "WorldCell: " << ResourceHolder::GetAllByTypeName("WorldCell" ).size() << std::endl;
    std::cout << "GameObject: " << ResourceHolder::GetAllByTypeName("GameObject" ).size() << std::endl;
    std::cout << "GameEntity: " << ResourceHolder::GetAllByTypeName("GameEntity" ).size() << std::endl;
    std::cout << "Image: " << ResourceHolder::GetAllByTypeName("Image" ).size() << std::endl;
    std::cout << "WavefrontMaterial: " << ResourceHolder::GetAllByTypeName("WavefrontMaterial" ).size() << std::endl;
    std::cout << "Light: " << ResourceHolder::GetAllByTypeName("Light" ).size() << std::endl;
    std::cout << "Mesh: " << ResourceHolder::GetAllByTypeName("Mesh" ).size() << std::endl;
    ResourceHolder::Export( "world.binary" );
    /*
Attaching Utility library
Attaching AssetInterfaces library
Attaching AssetGenerics library
Attaching Assets library
UUIDManager::initialize ()
Initialize Assets Managers
Interface: [Image] --> [{80b90ea1-4539-0a94-ddb9-0ccb094cb446}]
Interface: [Material] --> [{559b1809-a799-ae25-d1bd-03cb51a8c902}]
Interface: [Mesh] --> [{1afd9661-9e85-bd2a-d96b-52ce8561271e}]
Interface: [Light] --> [{1179fdb0-2986-e729-d4a0-12420ab6436f}]
Interface: [GameEntity] --> [{ab89e749-399d-f6b2-972d-9737e7b3a207}]
Interface: [GameObject] --> [{4d990a95-eae9-8802-4195-595c9db74560}]
Interface: [WorldCell] --> [{cf3451c8-8ab6-8282-26cf-acf314e75c8f}]
Interface: [World] --> [{e60f58ac-4656-86f1-188a-e788121cfed7}]
Initialize Implementation Resource
Implementation: [Image] --> [{0685f590-f83a-0e1f-d272-2a46b8321d24}]
Implementation: [Mesh] --> [{4394cb82-98f0-f0ef-1e3a-e4956402ace7}]
Implementation: [Light] --> [{3cf07b05-6309-a9c5-fb18-c6991b107714}]
Implementation: [GameEntity] --> [{c2475906-4124-98ad-4077-daca4645492a}]
Implementation: [GameObject] --> [{e1bf9bb2-3648-1d98-96da-a6f5fc95a5fb}]
Implementation: [WorldCell] --> [{9855558d-bbf9-13a9-4946-8969a5bd7cc5}]
Implementation: [World] --> [{8139645d-dc0d-a852-d77d-89ffe3ae9079}]
Attaching Wavefront library
Implementation: [WavefrontMaterial] --> [{3c50697c-543a-0e7f-6ab6-23ae942b73cc}]
Initialize Wavefront Loaders
Attaching QImageLoader library
Initialize QImage Loaders
World: 1
WorldCell: 1
GameObject: 1390
GameEntity: 20
Image: 23
WavefrontMaterial: 133
Light: 6
Mesh: 84
Detaching QImageLoader library
Detaching Wavefront library
Detaching Assets library
Detaching AssetGenerics library
Detaching AssetInterfaces library
Detaching Utility library
*/
#else

    ResourceHolder::Import( "world.binary" );
    std::cout << "World: " << ResourceHolder::GetAllByTypeName("World" ).size() << std::endl;
    std::cout << "WorldCell: " << ResourceHolder::GetAllByTypeName("WorldCell" ).size() << std::endl;
    std::cout << "GameObject: " << ResourceHolder::GetAllByTypeName("GameObject" ).size() << std::endl;
    std::cout << "GameEntity: " << ResourceHolder::GetAllByTypeName("GameEntity" ).size() << std::endl;
    std::cout << "Image: " << ResourceHolder::GetAllByTypeName("Image" ).size() << std::endl;
    std::cout << "WavefrontMaterial: " << ResourceHolder::GetAllByTypeName("WavefrontMaterial" ).size() << std::endl;
    std::cout << "Light: " << ResourceHolder::GetAllByTypeName("Light" ).size() << std::endl;
    std::cout << "Mesh: " << ResourceHolder::GetAllByTypeName("Mesh" ).size() << std::endl;
#endif

    //std::cout << dump ( ResourceHolder::GetAllByTypeName("World" )[0].dynamicCast<Assets::World>() ).toStdString();

#if 0
    QMap < QUuid, ByteBuffer* > buffers;
    unsigned long long totalSize = 0;
    int i = 0;
    foreach ( QUuid uuid, ResourceHolder::AllKeys() ) {
        ByteBuffer* buffer = new ByteBuffer;
        *buffer = ResourceHolder::ToBuffer(ResourceHolder::GetByUUID(uuid));
        buffers [ uuid ] = buffer;
        totalSize += buffer->getLength();
        i ++;
        std::cout << "Convert to buffer: " << (100.0f * ( (float) i ) / ResourceHolder::AllKeys().size() ) << "%" << "            \r";
    }
    std::cout << std::endl;
    std::cout << ResourceHolder::AllKeys().size() << " :: " << totalSize << std::endl;
    ByteBuffer dictionnary ( sizeof ( unsigned long long ) );
    index = 0;
    std::ofstream out1;
    out1.open ( "world.binary" );
    foreach ( QUuid uuid, ResourceHolder::AllKeys() ) {
        ByteBuffer buuid = ::toBuffer ( uuid );
        ByteBuffer blength = ::toBuffer ( buffers [uuid]->getLength() );
        out1.write((char*)buffers[uuid]->getData(),buffers[uuid]->getLength() );
    }
    out1.close();
#endif
    return 0;
#if 0
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
        /*for ( int j = 0 ; j < cell->getSize < QUuid > ( "GameObjects" ) ; j ++ ) {
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
        }*/
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
#endif
}
