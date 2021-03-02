//
// Created by weining on 28/1/21.
//

#include "take2_math.hh"
#include "exam2lib.hh"

using OptError = std::optional< std::string >;

OptError ensure_it_compiles()
{
    std::ostringstream oss;
    ImageDriver id;
    Sphere sphere{ Vec3{ 99, 99, 99 }, 3, nullptr };
    Renderer renderer{};
    renderer.add( &sphere );
    if ( auto status = id.drive( Camera{ 1, 1 }, renderer ); status )
    {
        std::string expected{ R"(P3
4 4
255
255 255 255 255 255 255 255 255 255 255 255 255
255 255 255 255 255 255 255 255 255 255 255 255
255 255 255 255 255 255 255 255 255 255 255 255
255 255 255 255 255 255 255 255 255 255 255 255
)" };
        id.output( oss );
        assert( expected == oss.str() );
        return std::nullopt;
    }
    else
    {
        return "failed to render dummy image to text";
    }
}

OptError ensure_it_generate_background_color()
{
    ImageDriver id{ 10, 5, 1 };
    Renderer renderer{ DualTone{ Color{ 0.5, 0.7, 1 }, Color{ 1, 1, 1 } } };
    if ( auto status = id.drive( Camera{ 4.0, 2.0 }, renderer ); status )
    {
        std::ofstream ofs{ "/tmp/out.ppm" };
        id.output( ofs );
        return std::nullopt;
    }
    else
    {
        return "failed to render background color to file";
    }
}

OptError ensure_it_renders_single_sphere()
{
    ImageDriver id{ 30, 20, 1 };  // 3 : 2
    Lambertian diffuse{};
    Sphere sphere{ Vec3{ 0, 0, -1 }, 0.5, &diffuse };
    Sphere base{ Vec3{ 0, -100.5, -1 }, 100, &diffuse };
    Renderer renderer{ DualTone{ Color{ 0.5, 0.7, 1 }, Color{ 1, 1, 1 } } };
    renderer.add( &sphere );
    renderer.add( &base );
    if ( auto status = id.drive( Camera{ 3.0, 2.0 }, renderer ); status )
    {
        std::ofstream ofs{ "/tmp/1sphere.ppm" };
        id.output( ofs );
        return std::nullopt;
    }
    else
    {
        return "failed to render single sphere to file";
    }
}

OptError ensure_reflection_multiple_sphere()
{
    ImageDriver id{ 30, 20, 1 };  // 3 : 2
    Lambertian diffuseBlue{ { 0.4, 0.6, 1.0 } };
    Lambertian diffuseRed{ { 1.4, 0.6, 0.4 } };
    Lambertian diffuseGrey{};
    Metal metal{ { 1.0, 0.8, 0.7 }, 0.1 };
    Sphere s1{ Vec3{ -0.6 - 0.3, -0.2, -1 }, 0.3, &diffuseBlue };
    Sphere s2{ Vec3{ 0, 0, -1 }, 0.5, &metal };
    Sphere s3{ Vec3{ 0.6 + 0.3, -0.2, -1 }, 0.3, &diffuseRed };
    Sphere base{ Vec3{ 0, -100.5, -1 }, 100, &diffuseGrey };
    Renderer renderer{ DualTone{ Color{ 0.5, 0.7, 1 }, Color{ 1, 1, 1 } } };
    renderer.add( &s1 );
    renderer.add( &s2 );
    renderer.add( &s3 );
    renderer.add( &base );
    if ( auto status = id.drive( Camera{ 3.0, 2.0 }, renderer ); status )
    {
        std::ofstream ofs{ "/tmp/3spheres.ppm" };
        id.output( ofs );
        return std::nullopt;
    }
    else
    {
        return "failed to render 3 spheres to file";
    }
}

// NOTE: I've done some lookdev here and removed the glass sphere(s)
OptError ensure_refraction_and_glass_surface()
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
        return std::nullopt;
    }
    else
    {
        return "failed to render glass, metal and rubber sphere to file";
    }
}

OptError ensure_camera_has_fov()
{
    constexpr int x = 150;
    constexpr int y = 100;
    ImageDriver id{ x, y, 1 };
    Lambertian diffuseBlue{ { 0.4, 0.6, 1.0 } };
    Lambertian diffuseRed{ { 1.4, 0.6, 0.4 } };
    Lambertian diffuseGrey{};
    Metal metal{ { 0.9, 0.9, 0.9 }, 0.1 };
    Sphere s1{ Vec3{ -0.6 - 0.3, -0.2, -1 }, 0.3, &diffuseBlue };
    Sphere s2{ Vec3{ 0, 0, -1 }, 0.5, &metal };
    Sphere s3{ Vec3{ 0.6 + 0.3, -0.2, -1 }, 0.3, &diffuseRed };
    Sphere base{ Vec3{ 0, -100.5, -1 }, 100, &diffuseGrey };
    Renderer renderer{ DualTone{ Color{ 0.5, 0.7, 1 }, Color{ 1, 1, 1 } } };
    renderer.add( &s1 );
    renderer.add( &s2 );
    renderer.add( &s3 );
    renderer.add( &base );
    Camera camera{ //
                   Camera::ORIENTATION{},
                   45,
                   double( x ) / double( y ),
                   Vec3{ -2, 2, 1 },
                   Vec3{ 0, 0, -1 },
                   Vec3( 0, 1, 0 ) };
    if ( auto status = id.drive( camera, renderer ); status )
    {
        std::ofstream ofs{ "/tmp/3fov.ppm" };
        id.output( ofs );
        return std::nullopt;
    }
    else
    {
        return "failed to render 3 spheres with fov to file";
    }
}

OptError ensure_camera_has_depth_of_field()
{
    constexpr int x = 600;
    constexpr int y = 400;
    constexpr size_t ss = 32;

    ImageDriver id{ x, y, ss };
    Lambertian diffuseBlue{ { 0.4, 0.6, 1.0 } };
    Lambertian diffuseRed{ { 1.4, 0.6, 0.4 } };
    Lambertian diffuseGreen{ { 0.7, 0.7, 0 } };
    Metal metal{ { 0.9, 0.9, 0.9 }, 0.1 };

    Dielectrics dielectrics{};

    Sphere centerSphere{ Vec3{ 0, 0, -1 }, 0.5, &diffuseBlue };
    Sphere rightSphere{ Vec3{ 1, 0, -1 }, 0.5, &metal };
    Sphere leftSphereOuter{ Vec3{ -1, 0, -1 }, 0.5, &dielectrics };
    Sphere leftSphereInner{ Vec3{ -1, 0, -1 }, -0.45, &dielectrics };

    Sphere base{ Vec3{ 0, -100.5, -1 }, 100, &diffuseGreen };

    Renderer renderer{ DualTone{ Color{ 0.5, 0.7, 1 }, Color{ 1, 1, 1 } } };
    renderer.add( &centerSphere );
    renderer.add( &rightSphere );
    renderer.add( &leftSphereOuter );
    renderer.add( &leftSphereInner );

    renderer.add( &base );
    Vec3 position{ 3, 3, 2 };
    Vec3 lookAt{ 0, 0, -1 };
    Camera camera{ Camera::DOF{},
                   30,
                   double( x ) / double( y ),
                   position,
                   lookAt,
                   Vec3( 0, 1, 0 ),
                   2.0,
                   length( position - lookAt ) };
    if ( auto status = id.drive( camera, renderer ); status )
    {
        std::ofstream ofs{ "/tmp/3dof.ppm" };
        id.output( ofs );
        return std::nullopt;
    }
    else
    {
        return "failed to render 3 spheres with fov and depth of field to file";
    }
}

int main()
{
    if ( auto err = ensure_it_compiles(); err )
    {
        assert( false );
    }
    if ( auto err = ensure_it_generate_background_color(); err )
    {
        assert( false );
    }
    if ( auto err = ensure_it_renders_single_sphere(); err )
    {
        assert( false );
    }
    if ( auto err = ensure_reflection_multiple_sphere(); err )
    {
        assert( false );
    }
    if ( auto err = ensure_refraction_and_glass_surface(); err )
    {
        assert( false );
    }
    if ( auto err = ensure_camera_has_fov(); err )
    {
        assert( false );
    }
    if ( auto err = ensure_camera_has_depth_of_field(); err )
    {
        assert( false );
    }
    return 0;
}