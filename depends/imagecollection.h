/****************************************************************************
** Image collection for project 'depends'.
**
** Generated from reading image files: 
**      library.png
**
** Created: Di Mai 8 08:14:28 2007
**      by: The User Interface Compiler ($Id: qt/embed.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include <qimage.h>
#include <qdict.h>
#include <qmime.h>
#include <qdragobject.h>

// library.png
static const unsigned char image_0_data[] = {
    0x00,0x00,0x03,0x84,0x78,0x9c,0xfb,0xff,0xff,0x3f,0xc3,0x7f,0x0a,0x70,
    0x43,0x43,0xc3,0x7f,0x62,0x31,0x36,0xbd,0xc4,0x58,0x03,0x52,0xc7,0xc0,
    0xc0,0xf0,0x9f,0x5c,0x7b,0xb1,0xe9,0x05,0x81,0x03,0x07,0x0e,0xe0,0xa5,
    0x71,0xe9,0x25,0xa4,0x0f,0x44,0x8f,0xda,0x3b,0xb0,0xf6,0x92,0x9b,0x36,
    0x40,0x18,0x24,0x46,0x2c,0x46,0xd7,0x4b,0x0e,0x06,0x00,0x0b,0x65,0x88,
    0xb2
};

static struct EmbedImage {
    int width, height, depth;
    const unsigned char *data;
    ulong compressed;
    int numColors;
    const QRgb *colorTable;
    bool alpha;
    const char *name;
} embed_image_vec[] = {
    { 15, 15, 32, (const unsigned char*)image_0_data, 71, 0, 0, TRUE, "library.png" },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

static QImage uic_findImage( const QString& name )
{
    for ( int i=0; embed_image_vec[i].data; i++ ) {
	if ( QString::fromUtf8(embed_image_vec[i].name) == name ) {
	    QByteArray baunzip;
	    baunzip = qUncompress( embed_image_vec[i].data, 
		embed_image_vec[i].compressed );
	    QImage img((uchar*)baunzip.data(),
			embed_image_vec[i].width,
			embed_image_vec[i].height,
			embed_image_vec[i].depth,
			(QRgb*)embed_image_vec[i].colorTable,
			embed_image_vec[i].numColors,
			QImage::BigEndian
		);
	    img = img.copy();
	    if ( embed_image_vec[i].alpha )
		img.setAlphaBuffer(TRUE);
	    return img;
        }
    }
    return QImage();
}

class MimeSourceFactory_depends : public QMimeSourceFactory
{
public:
    MimeSourceFactory_depends() {}
    ~MimeSourceFactory_depends() {}
    const QMimeSource* data( const QString& abs_name ) const {
	const QMimeSource* d = QMimeSourceFactory::data( abs_name );
	if ( d || abs_name.isNull() ) return d;
	QImage img = uic_findImage( abs_name );
	if ( !img.isNull() )
	    ((QMimeSourceFactory*)this)->setImage( abs_name, img );
	return QMimeSourceFactory::data( abs_name );
    };
};

static QMimeSourceFactory* factory = 0;

void qInitImages_depends()
{
    if ( !factory ) {
	factory = new MimeSourceFactory_depends;
	QMimeSourceFactory::defaultFactory()->addFactory( factory );
    }
}

void qCleanupImages_depends()
{
    if ( factory ) {
	QMimeSourceFactory::defaultFactory()->removeFactory( factory );
	delete factory;
	factory = 0;
    }
}

class StaticInitImages_depends
{
public:
    StaticInitImages_depends() { qInitImages_depends(); }
#if defined(Q_OS_SCO) || defined(Q_OS_UNIXWARE)
    ~StaticInitImages_depends() { }
#else
    ~StaticInitImages_depends() { qCleanupImages_depends(); }
#endif
};

static StaticInitImages_depends staticImages;
