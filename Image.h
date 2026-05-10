#include <iostream>
#include <fstream>
#include <string>
#include "Matrix.h"
using namespace std;
struct pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    pixel() {
        red = green = blue = 0;
    }
    pixel(unsigned r, unsigned g, unsigned b) {
        red = r;
        green = g;
        blue = b;
    };
    bool operator==(const pixel& other) const
    {
        return (red == other.red && green == other.green && blue == other.blue);
    }
    
};
struct RLEPixel
{
    pixel pix;
    int count=0;
};
template<typename T>
class Image {
private:
    int width;
    int height;
    Matrix<pixel> pixels;
public:
    Image() : height(0),width(0), pixels(0, 0) {};
    Image(int w, int h) : height(h), width(w), pixels(h, w) {};

    pixel getPixel(int x, int y) const
    {
        return pixels(y, x);
    }
    void setPixel(int x, int y, pixel value)
    {
        pixels(y, x) = value;
    }
    bool load(const string& filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << "Error: Cannot open file\n";
            return false;
        }
        string magic;
        file >> magic;
        if (magic != "P3")
        {
            cout << " Only PPM format supported\n";
            return false;
        }
        int maxValue;
        file >> width >> height >> maxValue;
        pixels = Matrix<pixel>(height, width);
        int r,g,b;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                file >> r>>g>>b;
                pixels(i, j) = pixel((unsigned char)(r), (unsigned char)(g), (unsigned char)(b) );
            }
        }
        file.close();
        return true;
    }
    bool save(const string& filename) const
    {
        ofstream file(filename);
        if (!file)
        {
            cout << "Error: Cannot create file\n";
            return false;
        }
        file << "P3\n";
        file << width << " " << height << "\n";
        file << "255\n";
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                pixel p = pixels(i, j);
                file << (int)p.red << " " << (int)p.green << " " << (int)p.blue << " ";
            }
            file << "\n";
        }
        file.close();
        return true;
    }
   void negative()
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                pixels(i, j).red = 255 - pixels(i, j).red;
                pixels(i, j).green = 255 - pixels(i, j).green;
                pixels(i, j).blue = 255 - pixels(i, j).blue;
            }
        }
    }
    void adjustBrightness(int value)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
				pixel p = pixels(i, j);
				int r = (int)p.red + value;
				int g = (int)p.green + value;
				int b = (int)p.blue + value;

                if (r < 0) r = 0;
                if (r > 255) r = 255;
                if (g < 0) g = 0;
                if (g > 255) g = 255;
                if (b < 0) b = 0;
                if (b > 255) b = 255;
                pixels(i, j) = pixel((unsigned char)r, (unsigned char)g, (unsigned char)b);
            }
        }
    }
    void adjustContrast(int factor)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
				pixel p = pixels(i, j);
				int r = 128 + factor * (p.red - 128);
                int g = 128 + factor * (p.green - 128);
                int b = 128 + factor * (p.blue - 128);
                if (r < 0) r = 0;
                if (r > 255) r = 255;
                if (g < 0) g = 0;
                if (g > 255) g = 255;
                if (b < 0) b = 0;
                if (b > 255) b = 255;
                pixels(i, j) = pixel((unsigned char)r, (unsigned char)g, (unsigned char)b);
            }
        }
    }
    void scaling(int newwidth, int newheight) {
        Matrix<pixel> newpixel(newheight, newwidth);
        float xratio = (float)width / newwidth;
        float yratio = (float)height / newheight;
        for (int i = 0; i < newheight; i++)
        {
            for (int j = 0; j < newwidth; j++)
            {
                int oldx = j * xratio;
                int oldy = i * yratio;
                newpixel(i, j) = pixels(oldy, oldx);
            }
        }
        pixels = newpixel;
        width = newwidth;
        height = newheight;
    }
   void threshold(unsigned char thresh)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int gray = (pixels(i, j).red + pixels(i, j).green + pixels(i, j).blue) / 3;

                unsigned char value =
                    (gray >= thresh) ? 255 : 0;
                pixels(i, j).red = value;
                pixels(i, j).green = value;
                pixels(i, j).blue = value;
            }
        }
    }
    void Grayscale() {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                pixel p = pixels(i, j);
                unsigned char gray = static_cast<unsigned char>(0.299 * p.red + 0.587 * p.green + 0.114 * p.blue);
                pixels(i, j) = pixel(gray, gray, gray);
            }
		}
    }
    void sharpen()
    {
        Matrix<pixel> result(height, width);
        int kernel[3][3] =
        { { 0, -1,  0 },
            { -1, 5, -1 },
            { 0, -1,  0 }};
        for (int i = 1; i < height - 1; i++)
        {
            for (int j = 1; j < width - 1; j++)
            {
                int red = 0;
                int green = 0;
                int blue = 0;
                for (int ki = -1; ki <= 1; ki++)
                {
                    for (int kj = -1; kj <= 1; kj++)
                    {
                        pixel p = pixels(i + ki, j + kj);
                        int value = kernel[ki + 1][kj + 1];
                        red += p.red * value;
                        green += p.green * value;
                        blue += p.blue * value;
                    }
                }
                if (red < 0) red = 0;
                if (red > 255) red = 255;
                if (green < 0) green = 0;
                if (green > 255) green = 255;
                if (blue < 0) blue = 0;
                if (blue > 255) blue = 255;
                result(i, j) = pixel(
                    (unsigned char)red,
                    (unsigned char)green,
                    (unsigned char)blue
                );
            }
        }
        pixels = result;
    }
    void sobelEdgeDetection()
    {
        Matrix<pixel> result(height, width);
        int Gx[3][3] =
        {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
        };
        int Gy[3][3] =
        {
            {-1, -2, -1},
            { 0,  0,  0},
            { 1,  2,  1}
        };
        for (int i = 1; i < height - 1; i++)
        {
            for (int j = 1; j < width - 1; j++)
            {
                int sumX = 0;
                int sumY = 0;
                for (int ki = -1; ki <= 1; ki++)
                {
                    for (int kj = -1; kj <= 1; kj++)
                    {
                        pixel p = pixels(i + ki, j + kj);
                        int gray =
                            0.299 * p.red +
                            0.587 * p.green +
                            0.114 * p.blue;
                        sumX += gray * Gx[ki + 1][kj + 1];
                        sumY += gray * Gy[ki + 1][kj + 1];
                    }
                }
                int edge = abs(sumX) + abs(sumY);
                if (edge > 255)
                    edge = 255;
                if (edge < 0)
                    edge = 0;
                result(i, j) = pixel(edge, edge, edge);
            }
        }
        pixels = result;
    }
	void flipHorizontal()
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width / 2; j++)
            {
                pixel temp = pixels(i, j);
                pixels(i, j) = pixels(i, width - j - 1);
                pixels(i, width - j - 1) = temp;
            }
        }
    }
    void flipVertical()
    {
        for (int i = 0; i < height / 2; i++)
        {
            for (int j = 0; j < width; j++)
            {
                pixel temp = pixels(i, j);
                pixels(i, j) = pixels(height - i - 1, j);
                pixels(height - i - 1, j) = temp;
            }
        }
    }
    RLEPixel* compressRLE(int& Size)
    {
        int maxsize = width * height;
        RLEPixel* compressed = new RLEPixel[maxsize];
        pixel current = pixels(0, 0);
        int count = 1;
        Size = 0;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (i == 0 && j == 0)
                    continue;
                pixel next = pixels(i, j);
                if (next == current)
                {
                    count++;
                }
                else
                {
                    compressed[Size].pix
                        = current;
                    compressed[Size].count
                        = count;
                    Size++;
                    current = next;
                    count = 1;
                }
            }
        }
        compressed[Size].pix
            = current;
        compressed[Size].count
            = count;
        Size++;
        return compressed;
    }
    bool saveCompressed(const string& filename)
    {
        int size;
        RLEPixel* compressed =
            compressRLE(size);
        ofstream file(filename, ios::binary);
        if (!file)
        {
            cout << "Cannot create file\n";
            delete[] compressed;
            return false;
        }
        file.write((char*)&width,sizeof(width));
        file.write((char*)&height,sizeof(height));
        file.write((char*)&size,sizeof(size));
        for (int i = 0; i < size; i++)
        {
            file.write((char*)&compressed[i],sizeof(RLEPixel) );
        }
        file.close();
        delete[] compressed;
        return true;
    }
    bool loadCompressed(const string& filename)
    {
        ifstream file(filename, ios::binary);
        if (!file)
        {
            cout << "Cannot open file\n";
            return false;
        }
        file.read((char*)&width, sizeof(width));
        file.read((char*)&height, sizeof(height));
        pixels = Matrix<pixel>(height, width);
        int size;
        file.read((char*)&size, sizeof(size));
        int row = 0;
        int col = 0;
        for (int i = 0; i < size; i++)
        {
            RLEPixel data;
            file.read((char*)&data, sizeof(RLEPixel));

            for (int j = 0;j < data.count;j++)
            {
                pixels(row, col)  = data.pix;
                col++;
                if (col >= width)
                {
                    col = 0;
                    row++;
                }
            }
        }
        file.close();
        return true;
    }
    int originalSize()
    {
        return width * height * 3;
    }
    int compressedSize()
    {
        int size;
        RLEPixel* data = compressRLE(size);
        int total = size * sizeof(RLEPixel);
        delete[] data;
        return total;
    }
    float compressionRate()
    {
        int original = originalSize();
        int compressed = compressedSize();
        return((float)(original - compressed) / original) * 100;
    }
};