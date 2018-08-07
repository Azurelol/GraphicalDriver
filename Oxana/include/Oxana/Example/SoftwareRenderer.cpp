#include "Examples.h"
#include "tgaimage.h"

namespace Oxana
{
	namespace Examples
	{
		//const std::string outputPath = "output.tga";
		// Based on the tutorial by Dimtry V. Sokolov
		// https://github.com/ssloy/tinyrenderer/wiki
		void SoftwareRendering(GUI& gui)
		{				
			static Image image(100, 100, sf::Color::Black);
			static auto lineColor = sf::Color::Red;

			gui.Add(TestRunner("Bresenham's Line Algorithm", [&](UnitTest& test)
			{
				// First attempt
				image.Reset();
				static auto lineV0 = [&](Vector2 a, Vector2 b, float t_increment, sf::Color color)
				{
					for (float t = 0.0f; t < 1.0f; t += t_increment)
					{
						Vector2 c = Vector2::Lerp(a, b, t);
						image.SetPixel(c.x, c.y, color);
					}
				};

				Vector2 a(13, 20), b(80, 40);
				lineV0(a, b, 0.01f, lineColor);
				test.AppendImage("First Attempt", image);

				// Second attempt
				image.Reset();
				lineV0(a, b, 0.1f, lineColor);
				test.AppendImage("Second Attempt", image);

				// Third attempt
				image.Reset();
				test.AppendLine("We need to swap the points so that x0 is always lowwer than x1. If the line is steep we transpose the image");
				test.AppendLine("We also have to make it always draw left to right");
				static auto lineV1 = [&](Vector2 a, Vector2 b, sf::Color color)
				{
					bool steep = Vector2::Steep(a, b);
					if (steep)
					{
						a.Tranpose();
						b.Tranpose(); 
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

				auto thirdAttempt = [&]()
				{
					lineV1(a, b, lineColor);
					Vector2 c(27, 35), d(77, 66);
					lineV1(c, d, sf::Color::Blue);
					test.AppendImage("Third Attempt", image);
				};
				test.Time(thirdAttempt);

				test.BeginTimer();
				{
					lineV1(a, b, lineColor);
					Vector2 c(27, 35), d(77, 66);
					lineV1(c, d, sf::Color::Blue);
					test.AppendImage("Third Attempt", image);
				}
				test.EndTimer();


			}));



		}

	}
}