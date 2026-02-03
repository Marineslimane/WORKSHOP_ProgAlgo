#include <sil/sil.hpp>
#include "random.hpp"

/* 
informations globales de compréhension : 
    glm::vec3 : type pour les couleurs
    image.pixels() : std::vector of glm::vec3 
    struct vec3 
    {
        float r;
        float g;
        float b;
    }
    composantes de couleurs color.r etc sont des float compris entre 0.f et 1.f
*/

void keep_green_only(sil::Image& image)
{
    for(glm::vec3& color : image.pixels())
    {
        color.r = 0.f;
        color.b = 0.f;
    }
}

void channels_swap(sil::Image& image)
{
    for(glm::vec3& color : image.pixels())
    {
        // swap manuel : 
        float color_b_copy {color.b};
        color.b = color.r;
        color.r = color_b_copy;

        // ou plus simple : std::swap(color.b, color.r);
    }
}

void black_and_white(sil::Image& image)
{
    for(glm::vec3& color : image.pixels())
    {
        float grey = 0.3*color.r + 0.59*color.g + 0.11*color.b; // luminance

        color = {grey, grey, grey};
        
    }
}

void negative(sil::Image& image)
{
    for(glm::vec3& color : image.pixels())
    {
       color.g = 1 - color.g;
       color.b = 1 - color.b;
       color.r = 1 - color.r;
    }
}

void hue(sil::Image& image)
{
    for (int x {0}; x < image.width(); ++x)
    {
        for (int y {0}; y < image.height(); ++y)
        {
            image.pixel(x, y) = glm::vec3{static_cast<float>(x) / static_cast<float>(image.width() - 1)};
        }
    }
}

void mirror(sil::Image& image)
{
    for (int x {0}; x < image.width()/2; ++x) // on divise par 2 sinon on swap 2 fois du coup ça change rien
    {
        for (int y {0}; y < image.height(); ++y)
        {
            // attention : les indices de x s'arrêtent à image.width() - 1
            std::swap(image.pixel(x, y), image.pixel(image.width() - 1 - x, y));
        }
    }
}

void noise(sil::Image& image)
{
    for(glm::vec3& color : image.pixels())
    {
        int choice {random_int(0, 2)}; // random_int(MIN, MAX+1)

        if (choice == 1) // dans ce cas, le pixel est remplacé
        {
            // choix aléatoire de la couleur :
            color.g = random_float(0.f, 1.f); // random_float(MIN, MAX)
            color.b = random_float(0.f, 1.f);
            color.r = random_float(0.f, 1.f);
        }
    }
}

void rotate90(sil::Image& image)
{
    sil::Image image_rotated{image.height(), image.width()};

    for (int x {0}; x < image.width(); ++x)
    {
        for (int y {0}; y < image.height(); ++y)
        {
            image_rotated.pixel(y, image.width() - 1 - x) = image.pixel(x,y);
        }
    }

    image = image_rotated;
}

void rgb_split(sil::Image& image)
{
    sil::Image image_rgb_split{image.width(), image.height()};

    for (int x {0}; x < image.width()-2; ++x) 
    {
        for (int y {0}; y < image.height()-2; ++y)
        {
            image_rgb_split.pixel(x,y).r = image.pixel(x+2,y).r;
            image_rgb_split.pixel(x,y).g = image.pixel(x,y).g;
            image_rgb_split.pixel(x,y).b = image.pixel(x-2,y).b;
        }
    }

    image = image_rgb_split;
}


int main()
{ // définit la graine

    // les accolades permettent aux images de n'exister que dans le bloc. de manière générale, en C++, tout bloc construit avec des accolades introduit une nouvelle portée. ici, cela permet d'éviter les conflits de noms et c'est aussi plus lisible
    {
        sil::Image image{"images/logo.png"}; // lis l'image
        keep_green_only(image);
        image.save("output/keep_green_only.png"); // sauvegarde l'image
    }

    {
        sil::Image image{"images/logo.png"};
        channels_swap(image);
        image.save("output/channels_swap.png"); 
    }

    {
        sil::Image image{"images/logo.png"};
        black_and_white(image);
        image.save("output/black_and_white.png"); 
    }

    {
        sil::Image image{300/*width*/, 200/*height*/}; // crée une image noire
        hue(image);
        image.save("output/hue.png"); 
    }

    {
        sil::Image image{"images/logo.png"}; 
        mirror(image);
        image.save("output/mirror.png"); 
    }

    {
        sil::Image image{"images/logo.png"}; 
        noise(image);
        image.save("output/noise.png"); 
    }

    {
        sil::Image image{"images/logo.png"};
        rotate90(image);
        image.save("output/rotate90.png"); 
    }

    {
        sil::Image image{"images/logo.png"};
        rgb_split(image);
        image.save("output/rgb_split.png"); 
    }
}