#include "Examples.h"
#include "tgaimage.h"

namespace Oxana
{
	namespace Examples
	{
		const TGAColor white = TGAColor(255, 255, 255, 255);
		const TGAColor red = TGAColor(255, 0, 0, 255);
		const std::string outputPath = "output.tga";

		TGAColor Convert(sf::Color color)
		{
			return TGAColor(color.r, color.g, color.b, color.a);
		}

		class ImageRender
		{
			TGAImage image;
			float t_increment;
				
			public:
			ImageRender(float width, float height, float t_increment = 0.1f) : image(width, height, TGAImage::RGB)
			{				
				this->t_increment = t_increment;
			}

			void Save(const std::string& path)
			{
				image.write_tga_file(path.c_str());
			}

			void Clear()
			{
				image.clear();
			}

			void SetPixel(int x, int y, TGAColor color)
			{
				image.set(x, y, color);
			}

			void FlipVertically()
			{
				image.flip_vertically();
			}

			void FlipHorizontally()
			{
				image.flip_horizontally();
			}

			// Line algorithms
			void LineV0(Vector2 a, Vector2 b, TGAColor color)
			{
				for (float t = 0.0f; t < 1.0f; t += t_increment)
				{
					Vector2 c = Vector2::Lerp(a, b, t);
					image.set(c.x, c.y, color);
				}
			}

		};

		// Based on the tutorial by Dimtry V. Sokolov
		// https://github.com/ssloy/tinyrenderer/wiki
		void SoftwareRendering(GUI& gui)
		{	
			static ImageRender image(100, 100);

			gui.Add(Test("Bresenham's Line Algorithm", [&](UnitTest& test)
			{
				image.Clear();

				// First attempt
				static auto lineV0 = [&](Vector2 a, Vector2 b, float t_increment, TGAColor color)
				{
					for (float t = 0.0f; t < 1.0f; t += t_increment)
					{
						Vector2 c = Vector2::Lerp(a, b, t);
						image.SetPixel(c.x, c.y, color);
					}
				};

				Vector2 a(13, 20), b(80, 40);
				lineV0(a, b, 0.01f, red);
				image.Save(outputPath);
				test.AppendImage("First Attempt", outputPath);

				// Second attempt
				image.Clear();
				lineV0(a, b, 0.1f, red);
				image.Save(outputPath);
				test.AppendImage("Second Attempt", outputPath);

				// Third attempt
				test.AppendLine("We need to swap the points so that x0 is always lowwer than x1. If the line is steep we transpose the image");
				test.AppendLine("We also have to make it always draw left to right");
				static auto lineV1 = [&](Vector2 a, Vector2 b, TGAColor color)
				{
					bool steep = Vector2::Steep(a, b);
					if (steep)
					{
						a.TransposeInPlace();
						b.TransposeInPlace(); 
					}

					if (a.x > b.x)
						std::swap(a, b);

					for (int x = a.x; x <= b.x; x++)
					{
						float t = (x - a.x) / static_cast<float>(b.x - a.x);
						int y = a.y * (1.0f - t) + b.y * t;
						if (steep)
							image.SetPixel(y, x, color);
						else
							image.SetPixel(x, y, color);
					}
				};
				image.Clear();
				lineV1(a, b, red);
				Vector2 c(27, 35), d(77, 66);
				lineV1(c, d, Convert(sf::Color::Blue));
				image.Save(outputPath);
				test.AppendImage("Third Attempt", outputPath);




			}));



		}

	}
}