#include <sil/sil.hpp> // pour manipuler les images
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // pour PI, cos, sin
#include <glm/gtx/matrix_transform_2d.hpp> // pour rotate()
#include <algorithm> // pour sort(), max_element(), min_element()
#include <complex> // pour les nb complexes
#include "random.hpp"

/* VARIABLES GLOBALES */
float pi = glm::pi<float>();

/* INFORMATIONS GLOBALES DE COMPRÉHENSION PERSONNELLE

    glm::vec3 : vecteur à 3 composantes, type des couleurs
    image.pixels() : std::vector of glm::vec3 

    struct vec3 
    {
        float r;
        float g;
        float b;
    }

    les composantes des couleurs sont des float entre 0.f et 1.f
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
        float temp {color.b};
        color.b = color.r;
        color.r = temp;

        // swap préfait : std::swap(color.b, color.r);
    }
}

float get_brightness(glm::vec3& color)
{
    return 0.3*color.r + 0.59*color.g + 0.11*color.b; // formule de la luminance
}

void black_and_white(sil::Image& image)
{
    for(glm::vec3& color : image.pixels())
    {
        float bw_color = get_brightness(color); // on obtient la couleur en noir et blanc avec la luminance

        color = glm::vec3{bw_color};
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

void gradation(sil::Image& image)
{
    for (int x {0}; x < image.width(); ++x)
    {
        for (int y {0}; y < image.height(); ++y)
        {
            image.pixel(x, y) = glm::vec3{static_cast<float>(x) / static_cast<float>(image.width() - 1)};
        }
    }
}

/* MIROIRS : */
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

sil::Image mirror_return(sil::Image& image)
{
    sil::Image result{image};

    for (int x {0}; x < image.width()/2; ++x) // on divise par 2 sinon on swap 2 fois du coup ça change rien
    {
        for (int y {0}; y < image.height(); ++y)
        {
            // attention : les indices de x s'arrêtent à image.width() - 1
            std::swap(result.pixel(x, y), result.pixel(image.width() - 1 - x, y));
        }
    }

    return result;
}

sil::Image horizontal_mirror_return(sil::Image& image)
{
    sil::Image result{image};

    for (int x {0}; x < image.width(); ++x) // on divise par 2 sinon on swap 2 fois du coup ça change rien
    {
        for (int y {0}; y < image.height()/2; ++y)
        {
            // attention : les indices de y s'arrêtent à image.height() - 1
            std::swap(result.pixel(x, y), result.pixel(x, image.height() - 1 - y));
        }
    }

    return result;
}
/* FIN MIROIRS */

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
    sil::Image image_rotated{image.height(), image.width()}; // l'image finale n'a pas les mêmes dimensions que celle de base donc on crée une nouvelle image

    for (int x {0}; x < image.width(); ++x)
    {
        for (int y {0}; y < image.height(); ++y)
        {
            image_rotated.pixel(image.height() - 1 - y, x) = image.pixel(x,y); // rotation dans le sens direct
        }
    }

    image = image_rotated;
}

void rgb_split(sil::Image& image)
{
    sil::Image image_rgb_split{image.width(), image.height()}; // on crée une nouvelle image

    const int pixel_gap {30}; // j'ai créé cette variable pour pouvoir changer facilement le décalage de pixels
    // plus pixel_gap est petit, plus l'effet est subtile

    for (int x {pixel_gap}; x < image.width(); ++x) 
    {
        for (int y {0}; y < image.height(); ++y)
        {
            image_rgb_split.pixel(x-pixel_gap, y).r = image.pixel(x, y).r; // rouge d'un pixel un peu à sa droite
            image_rgb_split.pixel(x-pixel_gap, y).g = image.pixel(x-pixel_gap, y).g; // propre vert
            image_rgb_split.pixel(x-pixel_gap+1, y).b = image.pixel(x-pixel_gap, y).b; // bleu d'un pixel un peu à sa gauche
        }
    }

    image = image_rgb_split;
}

/* MODIFIER LUMINOSITÉ */
void brightness_low(sil::Image& image)
{

    const float p {1.5}; // plus p est grand, plus les couleurs sont assombries

    for(glm::vec3& color : image.pixels())
    {
        if ((color.g + color.b + color.r != 0) && (color.g + color.b + color.r != 1)) // sauf pour les noirs et blancs purs
        {
            color.g = std::pow(color.g, p);
            color.b = std::pow(color.b, p);
            color.r = std::pow(color.r, p);
        }
    }
}

void brightness_high(sil::Image& image)
{

    const float p {0.5}; // plus p est petit, plus les couleurs sont éclaircies

    for(glm::vec3& color : image.pixels())
    {
        if ((color.g + color.b + color.r != 0) && (color.g + color.b + color.r != 1)) // sauf pour les noirs et blancs purs
        {
            color.g = std::pow(color.g, p);
            color.b = std::pow(color.b, p);
            color.r = std::pow(color.r, p);
        }
    }
}
/* FIN MODIFIER LUMINOSITÉ */

/* DISQUE, CERCLES... */
void disc(sil::Image& image)
{
    // coordonnées du centre du cercle : 
    const int x_center {image.width()/2};
    const int y_center {image.height()/2};

    // rayon (j'ai divisé la largeur en 3 puis redivisé par 2 pour avoir un rayon et non diamètre)
    const int radius {image.width()/6};

    const int squared_radius {radius*radius};

    for (int x {0}; x < image.width(); ++x) 
    {
        for (int y {0}; y < image.height(); ++y)
        {
            if (squared_radius >= (x-x_center)*(x-x_center) + (y-y_center)*(y-y_center)) // condition d'appartenance du point au disque
            {
                // le point est colorié en blanc : 
                image.pixel(x,y).r = 1;
                image.pixel(x,y).g = 1;
                image.pixel(x,y).b = 1;
            }
        }
    }
}

void circle(sil::Image& image, const int x_center, const int y_center)
{
    // rayon (j'ai divisé la largeur en 3 puis redivisé par 2 pour avoir un rayon et non diamètre)
    const int radius {image.width()/6};

    const int squared_radius {radius*radius};
    const int thickness {5}; // épaisseur des traits du cercle
    const int squared2 {(radius - thickness)*(radius - thickness)};
    

    for (int x {0}; x < image.width(); ++x) 
    {
        for (int y {0}; y < image.height(); ++y)
        {
            if ((squared_radius >= (x-x_center)*(x-x_center) + (y-y_center)*(y-y_center)) && (squared2 <= (x-x_center)*(x-x_center) + (y-y_center)*(y-y_center))) // condition d'appartenance du point au cercle, en prenant en compte l'épaisseur
            {
                // le point est colorié en blanc : 
                image.pixel(x,y).r = 1;
                image.pixel(x,y).g = 1;
                image.pixel(x,y).b = 1;
            }
        }
    }
}

void animation(sil::Image& image, int i) // même fonction que disc() mais avec contrôle de la position du centre du disque cette fois
{
    // coordonnées du centre du cercle : 
    const int x_center {image.width()/i*2}; // cette fois-ci, x_center peut être modifié en fonction de i pour avoir un centre qui se déplace sur l'axe des x pendant l'animation
    const int y_center {image.height()/2};

    // rayon (j'ai divisé la largeur en 3 puis redivisé par 2 pour avoir un rayon et non diamètre)
    const int radius {image.width()/6};

    const int squared_radius {radius*radius};

    for (int x {0}; x < image.width(); ++x) 
    {
        for (int y {0}; y < image.height(); ++y)
        {
            if (squared_radius >= (x-x_center)*(x-x_center) + (y-y_center)*(y-y_center)) // condition d'appartenance du point au cercle
            {
                // le point est colorié en blanc : 
                image.pixel(x,y).r = 1;
                image.pixel(x,y).g = 1;
                image.pixel(x,y).b = 1;
            }
        }
    }
}

void rosacea(sil::Image& image)
{
    const int radius {image.width()/6};

    // coordonnées du centre du cercle central : 
    const int x_center {image.width()/2};
    const int y_center {image.height()/2};

    circle(image, x_center, y_center); // cercle central

    // autres cercles : 
    for (float teta {0}; teta < 2*pi ; teta += pi/3)
    {
        circle(image, x_center + radius*glm::cos(teta), y_center + radius*glm::sin(teta)); // coordonnées des nouveaux centres obtenus avec la trigo
    }
}
/* FIN DISQUE, CERCLES... */

/* MOSAIQUES : */
std::vector<glm::vec3>& tile(sil::Image& image, sil::Image& final, int start_tile_x, int start_tile_y)
{
    for (int x {0}; x <  image.width() ; x++)
    {
        for (int y {0}; y <  image.height() ; y++)
        {
            final.pixel(start_tile_x + x, start_tile_y + y) = image.pixel(x,y);
        }
    }

    return final.pixels();
}

void mosaic(sil::Image& image)
{
    // dimensions de l'image finale : 
    const int final_width {image.width()*5};
    const int final_height {image.height()*5};

    sil::Image final{final_width, final_height}; // image finale

    for (int x {0}; x <  5 ; x++)
    {
        for (int y {0}; y <  5 ; y++)
        {
            // coordonnées du coin gauche supérieur du carreau :
            int start_tile_x {x*image.width()};
            int start_tile_y {y*image.height()};

            // remplissage du carreau avec l'image qu'on souhaite répeter : 
            for (int x {0}; x <  image.width() ; x++)
            {
                for (int y {0}; y <  image.height() ; y++)
                {
                    final.pixel(start_tile_x + x, start_tile_y + y) = image.pixel(x,y);
                }
            }
        }
    }

    image = final;
}

void mirror_mosaic(sil::Image& image)
{
    // dimensions de l'image finale : 
    const int final_width {image.width()*5};
    const int final_height {image.height()*5};

    sil::Image final{final_width, final_height}; // image finale

    // différente possibilités de carreaux pour la mosaique : 
    sil::Image vertical_mirror{mirror_return(image)};
    sil::Image horizontal_mirror{horizontal_mirror_return(image)};
    sil::Image horizontal_mirror_of_vertical_mirror{horizontal_mirror_return(vertical_mirror)};

    sil::Image choice{image}; // va servir à faire les choix des carreaux

    for (int i {0}; i <  5 ; i++)
    {
        for (int j {0}; j <  5 ; j++)
        {
            // coordonnées du coin gauche supérieur du carreau :
            int start_tile_x {i*image.width()};
            int start_tile_y {j*image.height()};

            // choix de l'image à afficher dans le carreau de la mosaique en fonction de la parité de l'indice de la ligne et/ou de la colonne :
            if (i%2 != 0 && j%2 != 0)
            {
                choice = horizontal_mirror_of_vertical_mirror;
            }
            else if (j%2 != 0)
            {
               choice = horizontal_mirror;
            }
            else if (i%2 != 0)
            {
                choice = vertical_mirror;
            }
            else
            {
                choice = image;
            }

            // remplissage du carreau avec l'image qu'on souhaite répeter : 
            for (int x {0}; x <  image.width() ; x++)
            {
                for (int y {0}; y <  image.height() ; y++)
                {
                    final.pixel(start_tile_x + x, start_tile_y + y) = choice.pixel(x,y);
                }
            }
        }
    }

    image = final;
}
/* FIN MOSAIQUES : */

void effet_random(sil::Image& image)
{
    // effet random obtenu en essayant de faire le glich
    // je le trouve plutôt stylé même si c'est un fail alors je le laisse

    for (int x {0}; x < image.width()/2; ++x) // on divise par 2 sinon on swap 2 fois du coup ça change rien
    {
        for (int y {0}; y < image.height()/2; ++y)
        {
            int choice {random_int(0, 2)}; // random_int(MIN, MAX+1)
            int random_x {random_int(0, image.width())};
            int random_y {random_int(0, image.height())};

            if (choice == 1) 
            {
                for (int i {0}; i < random_x; ++i)
                {
                    for (int j {0}; j < random_y; ++j)
                    {
                        std::swap(image.pixel(x, y), image.pixel(i, j));
                    }
                }
            }
        }
    }

}

void glitch(sil::Image& image)
{
    const int nb_rectangles {100}; // contrôle le nombre de rectangles ie l'ampleur du glitch : plus nb_rectangles est grand, plus l'effet est accentué

    for (int i {0} ; i < nb_rectangles ; i++)
    {
        // génération aléatoire des rectangles qui vont être échangés : 

        // coordonnées du coin en haut à gauche du rectangle 1 : 
        int random_x1 {random_int(0, image.width())};
        int random_y1 {random_int(0, image.height())};
        // coordonnées du coin en haut à gauche du rectangle 2 : 
        int random_x2 {random_int(0, image.width())};
        int random_y2 {random_int(0, image.height())};

        // dimensions aléatoires des rectangles :
        int random_width {random_int(4, 15)};
        int random_height {random_int(3, 7)};

        // on vérifie que les indices sont correctement définis et on regénère aléatoirement sinon
        while (random_x1 + random_width >= image.width() || random_x2 + random_width >= image.width() || random_y1 + random_height >= image.height() || random_y2 + random_height >= image.height())
        {
            random_x1 = random_int(0, image.width());
            random_y1 = random_int(0, image.height());
            random_x2 = random_int(0, image.width());
            random_y2 = random_int(0, image.height());
        }

        // échange des rectangles : 
        for (int i {0}; i < random_width; ++i)
        {
            for (int j {0}; j < random_height; ++j)
            {
                std::swap(image.pixel(random_x1 + i, random_y1 + j), image.pixel(random_x2 + i, random_y2 + j));
            }
        }
    }
}

void pixels_sorting(sil::Image& image)
{
    // comme image.pixels() n'est pas une matrice, j'ai reproduit un parcourt matriciel en jouant avec les indices

    const int left_gap {6}; // plus left_gap est grand, plus image.width()/left_gap est petit et donc plus les indices de début pour le choix des sous listes de pixels à trier auront de chance d'être petit i.e. plus la sous-liste sera proche du bord gauche de l'image, pour chaque ligne

    for(int i{0} ; i < image.height() ; i++) // pour toutes les lignes
    {
        int nb_line = i*image.width(); // pour passer à la ligne suivante

        // génération d'indices aléatoires de début et de fin pour le tri des sous listes de pixels (pour chaque ligne, on prend un sous-tableau de pixels de la ligne et on le trie)
        int random_begin {random_int(nb_line, nb_line + image.width()/left_gap)};
        int random_end {random_int(random_begin+1, nb_line + image.width())};

        // on vérifie que les indices sont bien corrects/pas out of range :
        while(random_end > image.pixels().size()-1 || random_begin > image.pixels().size()-1)
        {
            random_begin = random_int(nb_line, nb_line + image.width()/left_gap);
            random_end = random_int(random_begin+1, nb_line + image.width());
        }

        // tri de la liste :
        std::sort(image.pixels().begin() + random_begin, image.pixels().begin() + random_end, [](glm::vec3 const& color1, glm::vec3 const& color2)
        {
            return color1.r + color1.g + color1.b < color2.r + color2.g + color2.b; // tri par couleur
        });
    }
}

float map(float value, float current_min, float current_max, float desired_min, float desired_max) // transforme la valeur value comprise entre current_min et current_max en une valeur comprise entre desired_min et desired_max et la renvoie
{
    if (current_max == current_min)
    {
        return desired_min;
    }

    return desired_min + (desired_max - desired_min) * (value - current_min) / (current_max - current_min);
}

void mandelbrot_fractal(sil::Image& image)
{
    int max_iter {100}; // nb maximum d'itérations pour le calcul de z

    for (int x {0}; x < image.width(); ++x)
    {
        for (int y {0}; y < image.height(); ++y)
        {
            int max_k {0}; 

            std::complex<float> c {map(x, 0, image.width() - 1, -2, 2), map(y, 0, image.height() - 1, -2, 2)};
            std::complex<float> z {0, 0};

            for (int k {0} ; k < max_iter ; k++)
            {
                z = z*z + c;

                if (std::abs(z) > 2.0) // condition de trop grande augmentation de z 
                {
                    max_k = k;
                    break; // on sort de la boucle
                }
            }

            float color {1.0f}; 

            // choix de la couleur - à refaire : 
            if (max_k == max_iter) 
            {
                color = 1.0f;     
            }
            else if (max_k <= 5) 
            {
                color = 0.1f;      
            }
            else if (max_k <= 10) 
            {
                color = 0.25f;    
            }
            else if (max_k <= 20) 
            {
                color = 0.4f;     
            }
            else if (max_k <= 40) 
            {
                color = 0.6f;
            }
            else if (max_k <= 60) 
            {
                color = 0.75f;
            }
            else if (max_k <= 70) 
            {
                color = 0.9f; 
            }
            else 
            {
                color = 1.0f;
            }

            image.pixel(x,y) = glm::vec3(color);
        }
    }
}

/* DÉGRADÉS COULEURS : */
void color_gradation(sil::Image& image) // version naïve
{
    glm::vec3 red {1,0,0};
    glm::vec3 green {0,1,0};

    for (int x {0}; x < image.width(); ++x)
    {
        for (int y {0}; y < image.height(); ++y)
        {
            glm::vec3 mix {glm::mix(red, green, static_cast<float>(x) / static_cast<float>(image.width() - 1))};
            image.pixel(x,y) = mix;
        }
    }
}

/* fonctions de conversion */
float sRGB_to_linearRGB(float sRGB_value)
{
    if (sRGB_value <= 0.04045f) 
    {
        return sRGB_value / 12.92f;
    } 
    else 
    {
        return pow((sRGB_value + 0.055f) / 1.055f, 2.4f);
    }
}

float linearRGB_to_sRGB(float linear_value)
{
    if (linear_value <= 0.0031308f) 
    {
        return linear_value * 12.92f;
    } 
    else 
    {
        return 1.055f * pow(linear_value, 1.0f / 2.4f) - 0.055f;
    }
}

glm::vec3 sRGB_to_linearRGB_color(const glm::vec3& linear_color)
{
    return glm::vec3(sRGB_to_linearRGB(linear_color.r), sRGB_to_linearRGB(linear_color.g), sRGB_to_linearRGB(linear_color.b));
}

glm::vec3 linearRGB_to_sRGB_color(const glm::vec3& linear_color)
{
    return glm::vec3(linearRGB_to_sRGB(linear_color.r), linearRGB_to_sRGB(linear_color.g), linearRGB_to_sRGB(linear_color.b));
}

glm::vec3 linearRGB_to_oklab(const glm::vec3& c) 
{
    float l = 0.4122214708f * c.r + 0.5363325363f * c.g + 0.0514459929f * c.b;
	float m = 0.2119034982f * c.r + 0.6806995451f * c.g + 0.1073969566f * c.b;
	float s = 0.0883024619f * c.r + 0.2817188376f * c.g + 0.6299787005f * c.b;

    float l_ = cbrtf(l);
    float m_ = cbrtf(m);
    float s_ = cbrtf(s);

    return 
    {
        0.2104542553f*l_ + 0.7936177850f*m_ - 0.0040720468f*s_,
        1.9779984951f*l_ - 2.4285922050f*m_ + 0.4505937099f*s_,
        0.0259040371f*l_ + 0.7827717662f*m_ - 0.8086757660f*s_,
    };
}

glm::vec3 oklab_to_linearRGB(const glm::vec3& c) 
{
    float l_ = c.r + 0.3963377774f * c.g + 0.2158037573f * c.b;
    float m_ = c.r - 0.1055613458f * c.g - 0.0638541728f * c.b;
    float s_ = c.r - 0.0894841775f * c.g - 1.2914855480f * c.b;

    float l = l_*l_*l_;
    float m = m_*m_*m_;
    float s = s_*s_*s_;

    return 
    {
		+4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
		-1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
		-0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
    };
}
/* FIN fonctions de conversion */

void oklab_gradation(sil::Image& image)
{
    glm::vec3 red {1,0,0};
    glm::vec3 green {0,1,0};
    // couleurs converties en Ok Lab :
    glm::vec3 ok_lab_red {linearRGB_to_oklab(sRGB_to_linearRGB_color(red))};
    glm::vec3 ok_lab_green {linearRGB_to_oklab(sRGB_to_linearRGB_color(green))};
    
    for (int x {0}; x < image.width(); ++x)
    {
        for (int y {0}; y < image.height(); ++y)
        {
            glm::vec3 mix {glm::mix(ok_lab_red, ok_lab_green, static_cast<float>(x) / static_cast<float>(image.width() - 1))};
            image.pixel(x,y) = mix;
        }
    }
}

void color_gradation_enhanced(sil::Image& image)
{
    oklab_gradation(image); // image dégradée avec couleurs en Ok Lab

    // retour en sRGB : 
    for (int x {0}; x < image.width(); ++x)
    {
        for (int y {0}; y < image.height(); ++y)
        {
           image.pixel(x,y) = linearRGB_to_sRGB_color(oklab_to_linearRGB(image.pixel(x,y)));
        }
    }
}
/* FIN DÉGRADÉS COULEURS */

void dithering(sil::Image& image)
{
    float bayer_matrix[4][4] = 
    {
        { 0,  8,  2, 10},
        {12,  4, 14,  6},
        { 3, 11,  1,  9},
        {15,  7, 13,  5}
    };

    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {   
            float brightness = get_brightness(image.pixel(x, y));

            int bx = x % 4;
            int by = y % 4;

            float threshold = (bayer_matrix[by][bx] + 0.5f) / 16.0f;
            
            if (brightness > threshold)
            {
                image.pixel(x, y) = glm::vec3(1, 1, 1); // blanc
            }
            else
            {
                image.pixel(x, y) = glm::vec3(0, 0, 0); // noir
            }
        }
    }
}

void histogram_normalization(sil::Image& image)
{
    std::vector<float> brightness_vect {}; // les array ne peuvent pas être utilisés car la taille n'est pas vraiment déterminée à la compilation pour image.pixels()

    // récupération de la luminosité de chaque pixel et stockage dans un tableau : 
    for(glm::vec3& color : image.pixels())
    {
        brightness_vect.push_back(get_brightness(color));
    }

    // à partir de ce tableau, calcul des maximum et minimum de luminosité des pixels
    float max_brightness {*std::max_element(brightness_vect.begin(), brightness_vect.end())};
    float min_brightness {*std::min_element(brightness_vect.begin(), brightness_vect.end())};

    float range = max_brightness - min_brightness; // étendue des valeurs de luminosité

    if (range == 0)
    {
        return; // pour ne pas diviser par 0
    }

    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            glm::vec3 color = image.pixel(x, y);

            float brightness = get_brightness(color);
            float difference = (brightness - min_brightness)/range;

            glm::vec3 new_color = color*(difference/brightness);

            image.pixel(x, y) = new_color;
        }
    }
}

glm::vec2 rotated(glm::vec2 point, glm::vec2 center_of_rotation, float angle)
{
    return glm::vec2{glm::rotate(glm::mat3{1.f}, angle) * glm::vec3{point - center_of_rotation, 0.f}} + center_of_rotation;
}

void vortex(sil::Image& image)
{
    glm::vec2 center_vortex {image.width()/2, image.height()/2}; // centre du vortex = centre de l'image

    float angle {5}; // valeur d'angle de début

    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            glm::vec2 point {x,y};

            float distance_to_center {glm::distance(point, center_vortex)};

            angle = distance_to_center*0.05f; // rotation proportionnelle à la distance au centre du vortex

            glm::vec2 rotated_point {rotated(point, center_vortex, angle)};
            
            // conversion en int car image.pixel() prend des int en paramètres et clamp pour s'assurer que les indices ne sont pas out of range
            int new_x = static_cast<int>(glm::clamp(rotated_point.x, 0.0f, image.width() - 1.0f));
            int new_y = static_cast<int>(glm::clamp(rotated_point.y, 0.0f, image.height() - 1.0f));
            
            image.pixel(x, y) = image.pixel(new_x, new_y);
        }
    }
}

void convolution(sil::Image& image)
{
    // kernel 3*3 1/9 (flou): 
    glm::mat3 blur_kernel {glm::mat3(1.0f / 9.0f)}; 

    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
    
            // gestion des trucs qui dépasse : on prend la valeur de pixel dans le truc (qui sort pas) la plus proche

            if (x == 0 || y ==0)
            {
                image.pixel(x,y) = 
            }
        }
    }
}

int main()
{
    // les accolades permettent aux images de n'exister que dans le bloc. de manière générale, en C++, tout bloc construit avec des accolades introduit une nouvelle portée. ici, cela permet d'éviter les conflits de noms et c'est aussi plus lisible
    // {
    //     sil::Image image{"images/logo.png"}; // lis l'image
    //     keep_green_only(image);
    //     image.save("output/keep_green_only.png"); // sauvegarde l'image
    // }

    // {
    //     sil::Image image{"images/logo.png"};
    //     channels_swap(image);
    //     image.save("output/channels_swap.png"); 
    // }

    // {
    //     sil::Image image{"images/logo.png"};
    //     black_and_white(image);
    //     image.save("output/black_and_white.png"); 
    // }

    // {
    //     sil::Image image{300/*width*/, 200/*height*/}; // crée une image noire
    //     gradation(image);
    //     image.save("output/gradation.png"); 
    // }

    // {
    //     sil::Image image{"images/logo.png"}; 
    //     mirror(image);
    //     image.save("output/mirror.png"); 
    // }

    // {
    //     sil::Image image{"images/logo.png"}; 
    //     noise(image);
    //     image.save("output/noise.png"); 
    // }

    // {
    //      sil::Image image{"images/logo.png"};
    //      rotate90(image);
    //      image.save("output/rotate90.png"); 
    // }

    // {
    //     sil::Image image{"images/logo.png"};
    //     rgb_split(image);
    //     image.save("output/rgb_split.png"); 
    // }

    // {
    //     sil::Image image{"images/photo.jpg"};
    //     brightness_low(image);
    //     image.save("output/brightness_low.png"); 
    // }

    // {
    //     sil::Image image{"images/photo.jpg"};
    //     brightness_high(image);
    //     image.save("output/brightness_high.png"); 
    // }

    // {
    //     sil::Image image{500/*width*/, 500/*height*/};
    //     disc(image);
    //     image.save("output/disc.png"); 
    // }

    // {
    //     sil::Image image{500/*width*/, 500/*height*/};
    //     circle(image, image.width()/2, image.height()/2);
    //     image.save("output/circle.png"); 
    // }

    // {
    //     int nb_frames {50}; // choix du nombre de frame ici

    //     for (int i {nb_frames} ; i > 1; i--)
    //     {
    //         sil::Image image{500/*width*/, 500/*height*/};
    //         animation(image, i);
    //         image.save("output/animation/frame_" + std::to_string(nb_frames - i) + ".png"); 
    //     }
    // }

    // {
    //     sil::Image image{500/*width*/, 500/*height*/};
    //     rosacea(image);
    //     image.save("output/rosacea.png"); 
    // }

    // {
    //     sil::Image image{"images/logo.png"};
    //     mosaic(image);
    //     image.save("output/mosaic.png"); 
    // }

    // {
    //     sil::Image image{"images/logo.png"};
    //     mirror_mosaic(image);
    //     image.save("output/mirror_mosaic.png"); 
    // }

    // {
    //     sil::Image image{"images/logo.png"};
    //     glitch(image);
    //     image.save("output/glitch.png"); 
    // }

    // {
    //     sil::Image image{"images/logo.png"};
    //     pixels_sorting(image);
    //     image.save("output/pixels_sorting.png"); 
    // }

    // {
    //     sil::Image image{500/*width*/, 500/*height*/};
    //     mandelbrot_fractal(image);
    //     image.save("output/mandelbrot_fractal.png"); 
    // }

    // {
    //     sil::Image image{500/*width*/, 500/*height*/};
    //     color_gradation(image);
    //     image.save("output/color_gradation.png"); 
    // }

    // {
    //     sil::Image image{500/*width*/, 500/*height*/};
    //     color_gradation_enhanced(image);
    //     image.save("output/oklab_gradation.png"); 
    // }

    // {
    //     sil::Image image{"images/photo.jpg"};
    //     dithering(image);
    //     image.save("output/dithering.png"); 
    // }

    // {
    //     sil::Image image{"images/photo_faible_contraste.jpg"};
    //     histogram_normalization(image);
    //     image.save("output/histogram_normalization.png"); 
    // }

    // {
    //     sil::Image image{"images/logo.png"};
    //     vortex(image);
    //     image.save("output/vortex.png"); 
    // }
}