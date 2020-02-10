#include <vpApp.h>
#include <vpObject.h>
#include <vpFxFlightRibbon.h>
#include <vpFxMissileTrail.h>
#include <vpMotionGame.h>
#include "vuAllocTracer.h"

vuAllocTracer s_allocTracer(true,true);

const float ORIGINX    = -178.0f;
const float ORIGINY    = -533.0f;
const float ORIGINZ    =  300.0f;
const float RAD_FLIGHT =  500.0f;
const float H_RATE     =  0.4f;
const float Z_RATE     = (RAD_FLIGHT * H_RATE);	

class myApp : public vpApp {

public:

    myApp() : 
      m_pPlane( NULL ), 
      m_pFlightRibbon( NULL )
    {}

    ~myApp()
    {
        m_pPlane->unref();
        m_pFlightRibbon->unref();
    }

    int configure() 
    {
        vpApp::configure();

        m_pPlane = vpObject::find( "myPlane" );
        assert(m_pPlane);
        m_pPlane->ref();

        m_pFlightRibbon = vpFxFlightRibbon::find( "myFlightRibbon" );
        assert(m_pFlightRibbon);
        m_pFlightRibbon->ref();

        return vsgu::SUCCESS;
    }

    virtual void run( void )
    {
         while ( beginFrame() != 0 )
         {
            float hsin, hcos, psin, pcos;
        
            m_hpr[0] += H_RATE;

            if ( m_hpr[0] > 180.0f )
                m_hpr[0] -= 180.0f;

            //if ( m_hpr[2] < 0.0f )
                //m_hpr[2] -= 180.0f;

            //vuSinCos( static_cast< float >( m_hpr[0] ), &hsin, &hcos );
            vuSinCos( static_cast< float >( m_hpr[1] ), &psin, &pcos );
            m_xyz[1] = ORIGINX + RAD_FLIGHT * hsin;
            m_xyz[1] = ORIGINY + RAD_FLIGHT * hsin;
            m_xyz[2] = ORIGINZ + Z_RATE     * psin;

            m_pPlane->setTranslate( m_xyz[0], m_xyz[1], m_xyz[2] ); //jalan
            m_pPlane->setRotate( m_hpr[0], m_hpr[1], m_hpr[2] ); //belok

            endFrame();
         }
    }

private:

    vpFxFlightRibbon *m_pFlightRibbon;

    vpObject *m_pPlane;

    vuVec3d m_hpr;
    vuVec3d m_xyz;

};

int main( int argc, char *argv[] )
{

    vp::initialize(argc, argv);

    myApp *app = new myApp;

    app->define( (argc <= 1) ? "vpfx_flight_ribbon.acf" : argv[1] );

    app->configure();

    app->run();

    app->unconfigure();

    app->unref();

    vp::shutdown();

    return 0;
}