//
// Created by weining on 2/3/21.
//

#include "take2_math.hh"
#include "exam2lib.hh"

// extracted from the main test unit as it can be useful

void ensure_refraction_and_glass_surface()
{
    constexpr size_t x = 400;
    constexpr size_t y = 200;
    constexpr size_t ss = 1;
    ImageDriver id{ x, y, ss };  // 3 : 2
    Lambertian diffuseBlue{ { 0.6, 0.66, 0.66 } };
    Metal metalBase{ { 0.82, 0.61, 0.53 }, 0.8 };
    Metal metalBronze{ { 0.68, 0.7, 0.4 }, 0.35 };
    Metal metalIron{ { 0.6, 0.6, 0.67 }, 0.06 };
    Dielectrics dielectrics{};

    Sphere s1{ Vec3{ 0, -0.004, -3.4 }, 0.5, &diffuseBlue };
    Sphere s2{ Vec3{ 1.2, -0.015, -3.95 }, 0.5, &metalBronze };
    Sphere s3{ Vec3{ -1.2, -0.015, -3.7 }, 0.5, &metalIron };

    Sphere base{ Vec3{ 0, -100.5, -3.6 }, 100, &metalBase };

    DualTone bg{ //
                 Color{ 0.09, 0.14, 0.45 },
                 Color{ 0.9, 0.5, 0.09 },
                 4 };
    Renderer renderer{ bg };
    renderer.add( &s1 );
    renderer.add( &s2 );
    renderer.add( &s3 );
    renderer.add( &base );
    Camera camera{ Camera::FOV{}, 45, double( x ) / double( y ) };
    if ( auto status = id.drive( camera, renderer ); status )
    {
        std::ofstream ofs{ "/tmp/3glass.ppm" };
        id.output( ofs );
    }
}

int main()
{
    ensure_refraction_and_glass_surface();
    return 0;
}