#include "arch.h"
#include "pif.h"

#include <stdio.h>


STATUS main( )
{
    STATUS Status;
    CHAR VendorString[16];
    CHAR BrandString[64];

    Status = PifInitialize( );
    if (!SUCCESS( Status ))
    {
        return Status;
    }

    Status = PifGetVendorString( VendorString, sizeof( VendorString ) );
    if (!SUCCESS( Status ))
    {
        return Status;
    }

    Status = PifGetBrandString( BrandString, sizeof( BrandString ) );
    if (!SUCCESS( Status ))
    {
        return Status;
    }
    
    printf( "Vendor is %s\n", VendorString );
    printf( "\t%s\n\n", BrandString );

    IsFeatureSupportedMessage( 3DNOW );
    IsFeatureSupportedMessage( 3DNOWEXT );
    IsFeatureSupportedMessage( ABM );
    IsFeatureSupportedMessage( ADX );
    IsFeatureSupportedMessage( AES );
    IsFeatureSupportedMessage( AVX );
    IsFeatureSupportedMessage( AVX2 );
    IsFeatureSupportedMessage( AVX512CD );
    IsFeatureSupportedMessage( AVX512F );
    IsFeatureSupportedMessage( AVX512PF );
    IsFeatureSupportedMessage( BMI1 );
    IsFeatureSupportedMessage( BMI2 );
    IsFeatureSupportedMessage( CLFSH );
    IsFeatureSupportedMessage( CMPXCHG16B );
    IsFeatureSupportedMessage( CMPXCHG8B );
    IsFeatureSupportedMessage( ERMS );
    IsFeatureSupportedMessage( F16C );
    IsFeatureSupportedMessage( FMA );
    IsFeatureSupportedMessage( FSGSBASE );
    IsFeatureSupportedMessage( FXSR );
    IsFeatureSupportedMessage( HLE );
    IsFeatureSupportedMessage( INVPCID );
    IsFeatureSupportedMessage( LAHF_LM );
    IsFeatureSupportedMessage( LZCNT );
    IsFeatureSupportedMessage( MMX );
    IsFeatureSupportedMessage( MMXEXT );
    IsFeatureSupportedMessage( MONITOR );
    IsFeatureSupportedMessage( VMX );
    IsFeatureSupportedMessage( SMX );
    IsFeatureSupportedMessage( EIST );
    IsFeatureSupportedMessage( MOVBE );
    IsFeatureSupportedMessage( MSR );
    IsFeatureSupportedMessage( OSXSAVE );
    IsFeatureSupportedMessage( PCLMULQDQ );
    IsFeatureSupportedMessage( POPCNT );
    IsFeatureSupportedMessage( PREFETCHWT1 );
    IsFeatureSupportedMessage( RDPID );
    IsFeatureSupportedMessage( RDRAND );
    IsFeatureSupportedMessage( RDSEED );
    IsFeatureSupportedMessage( RDTSCP );
    IsFeatureSupportedMessage( RTM );
    IsFeatureSupportedMessage( SEP );
    IsFeatureSupportedMessage( SHA );
    IsFeatureSupportedMessage( SSE );
    IsFeatureSupportedMessage( SSE2 );
    IsFeatureSupportedMessage( SSE3 );
    IsFeatureSupportedMessage( SSE41 );
    IsFeatureSupportedMessage( SSE42 );
    IsFeatureSupportedMessage( SSE4a );
    IsFeatureSupportedMessage( SSSE3 );
    IsFeatureSupportedMessage( SYSCALL );
    IsFeatureSupportedMessage( TBM );
    IsFeatureSupportedMessage( XOP );
    IsFeatureSupportedMessage( XSAVE );

    return Status;
}