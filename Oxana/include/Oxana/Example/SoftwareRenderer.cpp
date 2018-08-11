#include "Examples.h"
#include "tgaimage.h"

namespace Oxana
{
	namespace Examples
	{
		// Based on the tutorial by Dimtry V. Sokolov
		// https://github.com/ssloy/tinyrenderer/wiki
		void BresenhamsLineAlgorithm(GUI& gui);
		void TriangleRasterizationBackfaceCulling(GUI& gui);

		void SoftwareRendering(GUI& gui)
		{
			BresenhamsLineAlgorithm(gui);
			TriangleRasterizationBackfaceCulling(gui);
		}

		class Model
		{
			std::vector<Vector3f> vertices;
			std::vector<std::vector<int>> faces;

			void Parse(const std::string& fileName)
			{
				std::ifstream in;
				in.open(fileName, std::ifstream::in);
				if (in.fail()) return;
				std::string line;
				while (!in.eof())
				{
					std::getline(in, line);
					std::istringstream iss(line.c_str());
					char trash;
					if (!line.compare(0, 2, "v "))
					{
						iss >> trash;
						Vector3f v;
						for (int i = 0; i < 3; i++) iss >> v.raw[i];
						vertices.push_back(v);
					}
					else if (!line.compare(0, 2, "f "))
					{
						std::vector<int> f;
						int itrash, idx;
						iss >> trash;
						while (iss >> idx >> trash >> itrash >> trash >> itrash)
						{
							idx--; // in wavefront obj all indices start at 1, not zero
							f.push_back(idx);
						}
						faces.push_back(f);
					}
				}
			}

			public:
			Model(const std::string& fileName) { Parse(fileName); }
			Vector3f GetVertex(int index) { return vertices[index]; }
			std::vector<int> GetFace(int index) { return faces[index]; }
			int FaceCount() { return faces.size(); }
			int VertexCount() { return vertices.size(); }


		};



		void BresenhamsLineAlgorithm(GUI & gui)
		{
			static Image image(100, 100, sf::Color::Black);
			static auto lineColor = sf::Color::Red;

			gui.Add(TestRunner("Bresenham's Line Algorithm", [&](UnitTest& test)
			{
				// First attempt
				image.Reset(100, 100);
				static auto lineV0 = [&](Vector2f a, Vector2f b, float t_increment, sf::Color color)
				{
					for (float t = 0.0f; t < 1.0f; t += t_increment)
					{
						Vector2f c = Vector2f::Lerp(a, b, t);
						image.SetPixel(c.x, c.y, color);
					}
				};

				Vector2f a(13, 20), b(80, 40);
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
				static auto lineV1 = [&](Vector2f a, Vector2f b, sf::Color color)
				{
					bool steep = Vector2f::Steep(a, b);
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

				test.BeginTimer();
				{
					lineV1(a, b, lineColor);
					Vector2f c(27, 35), d(77, 66);
					lineV1(c, d, sf::Color::Blue);
					test.AppendImage("Third Attempt", image);
				}
				test.EndTimer();

				// Fourth attempt
				image.Reset();
				test.Append("Note that each division has the same divisor. We can take it out of the loop. An error variable can ");
				test.Append("give us the distance to the best straight line from the current (x,y) pixel. Each time error is greater ");
				test.Append("than one pixel, we increase (or decrease) y and the error by one\n");
				static auto lineV2 = [&](Vector2f a, Vector2f b, sf::Color color)
				{
					bool steep = Vector2f::Steep(a, b);
					if (steep)
					{
						a.Tranpose();
						b.Tranpose();
					}

					if (a.x > b.x)
						std::swap(a, b);

					int dx = b.x - a.x;
					int dy = b.y - a.y;
					float error = 0;
					float derror = std::abs((float)dy / dx);
					int y = a.y;

					for (int x = a.x; x <= b.x; x++)
					{
						if (steep)
							image.SetPixel(y, x, color);
						else
							image.SetPixel(x, y, color);

						error += derror;
						if (error > 0.5f)
						{
							y += (b.x > a.x ? 1 : -1);
							error -= 1;
						}
					}
				};
				test.BeginTimer();
				{
					lineV2(a, b, lineColor);
					Vector2f c(27, 35), d(77, 66);
					lineV2(c, d, sf::Color::Blue);
					test.AppendImage("Fourth Attempt", image);
				}
				test.EndTimer();

				// Final  attempt
				image.Reset();
				test.Append("We don't need floating points; we can replace the error variable with another one, ");
				test.Append("assuming it eual to (error * dx * 2)");
				static auto lineV3 = [&](Vector2f a, Vector2f b, sf::Color color)
				{
					bool steep = Vector2f::Steep(a, b);
					if (steep)
					{
						a.Tranpose();
						b.Tranpose();
					}

					if (a.x > b.x)
						std::swap(a, b);

					int dx = b.x - a.x;
					int dy = b.y - a.y;
					int error = 0;
					int derror = std::abs(dy) * 2;
					int y = a.y;

					for (int x = a.x; x <= b.x; x++)
					{
						if (steep)
							image.SetPixel(y, x, color);
						else
							image.SetPixel(x, y, color);

						error += derror;
						if (error > dx)
						{
							y += (b.x > a.x ? 1 : -1);
							error -= dx * 2;
						}
					}
				};
				test.BeginTimer();
				{
					lineV3(a, b, lineColor);
					Vector2f c(27, 35), d(77, 66);
					lineV3(c, d, sf::Color::Blue);
					test.AppendImage("Final Attempt", image);
				}
				test.EndTimer();

				// Model
				const int width = 800, height = 800;
				image.Reset(width, height);
				Model model("include/Oxana/Example/african_head.obj");
				static auto lineV1_b = [&](Vector2i a, Vector2i b, sf::Color color)
				{
					bool steep = Vector2i::Steep(a, b);
					if (steep)
					{
						std::swap(a.x, a.y);
						std::swap(b.x, b.y);
						//a.Tranpose();
						//b.Tranpose();
					}

					if (a.x > b.x)
						std::swap(a, b);

					for (int x = a.x; x <= b.x; x++)
					{
						float t = (x - a.x) / static_cast<float>(b.x - a.x);
						int y = a.y * (1.0f - t) + (b.y * t);
						if (steep)
							image.TrySetPixel(y, x, color);
						else
							image.TrySetPixel(x, y, color);
					}
				};

				static auto wiremesh = [&](Model model)
				{
					for (int i = 0; i < model.FaceCount(); i++)
					{
						std::vector<int> face = model.GetFace(i);
						for (int j = 0; j < 3; j++)
						{
							Vector3f v0 = model.GetVertex(face[j]);
							Vector3f v1 = model.GetVertex(face[(j + 1) % 3]);
							int x0 = (v0.x + 1.)*width / 2.;
							int y0 = (v0.y + 1.)*height / 2.;
							int x1 = (v1.x + 1.)*width / 2.;
							int y1 = (v1.y + 1.)*height / 2.;
							Vector2i a = Vector2i(x0, y0);
							Vector2i b = Vector2i(x1, y1);
							lineV1_b(a, b, sf::Color::Red);
						}
					}
				};

			#pragma region WorkingWiremesh
				static auto lineOld = [&](int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
				{
					bool steep = false;
					if (std::abs(x0 - x1) < std::abs(y0 - y1))
					{
						std::swap(x0, y0);
						std::swap(x1, y1);
						steep = true;
					}
					if (x0 > x1)
					{
						std::swap(x0, x1);
						std::swap(y0, y1);
					}

					for (int x = x0; x <= x1; x++)
					{
						float t = (x - x0) / (float)(x1 - x0);
						int y = y0 * (1. - t) + y1 * t;
						if (steep)
						{
							image.set(y, x, color);
						}
						else
						{
							image.set(x, y, color);
						}
					}
				};

				static auto wiremesh2 = [&]()
				{
					TGAImage tgaimage(width, height, TGAImage::RGB);
					for (int i = 0; i < model.FaceCount(); i++)
					{
						std::vector<int> face = model.GetFace(i);
						for (int j = 0; j < 3; j++)
						{
							Vector3f v0 = model.GetVertex(face[j]);
							Vector3f v1 = model.GetVertex(face[(j + 1) % 3]);
							int x0 = (v0.x + 1.)*width / 2.;
							int y0 = (v0.y + 1.)*height / 2.;
							int x1 = (v1.x + 1.)*width / 2.;
							int y1 = (v1.y + 1.)*height / 2.;
							lineOld(x0, y0, x1, y1, tgaimage, TGAColor(255, 0, 0, 255));
						}
					}
					tgaimage.flip_vertically(); // i want to have the origin at the left bottom corner of the image
					tgaimage.write_tga_file("output.tga");
					test.AppendImage("Wiremesh Render", "output.tga");
				};
			#pragma endregion
				
				wiremesh(model);
				test.AppendImage("Wiremesh Render", image);
				//wiremesh2();


			}));
		}
		void TriangleRasterizationBackfaceCulling(GUI & gui)
		{
			static Geometry geometry(200, 200, sf::Color::Black);
			gui.Add(TestRunner("Triangle Rasterization, Backface Culling", [&](UnitTest& test)
			{
				Vector2i t0[3] = { Vector2i(10, 70),   Vector2i(50, 160),  Vector2i(70, 80) };
				Vector2i t1[3] = { Vector2i(180, 50),  Vector2i(150, 1),   Vector2i(70, 180) };
				Vector2i t2[3] = { Vector2i(180, 150), Vector2i(120, 160), Vector2i(130, 180) };

				// 1
				geometry.Reset();
				static auto trianglev1 = [&](Vector2i a, Vector2i b, Vector2i c, Color color)
				{
					geometry.Line(a, b, color);
					geometry.Line(b, c, color);
					geometry.Line(a, c, color);
				};
				trianglev1(t0[0], t0[1], t0[2], sf::Color::Red);
				trianglev1(t1[0], t1[1], t1[2], sf::Color::Green);
				trianglev1(t2[0], t2[1], t2[2], sf::Color::Blue);
				test.AppendImage("Line Sweeping 1", geometry);
				test.AppendLine("Line sweeping:");
				test.AppendLine("1. Sort vertices of the triangle by their y-coordinates");
				test.AppendLine("2. Rasterize simultaneouisly the left and the right sides of the triangle");
				test.AppendLine("3. Draw an horizontal line segment between the left and the right boundary points");

				// 2
				geometry.Reset();
				static auto trianglev2 = [&](Vector2i a, Vector2i b, Vector2i c, Color color)
				{
					// Degenerate triangle
					if (a.y == b.y && a.y == c.y)
						return;

					// Sort the vertices a,b,c lower to upper (using bubblesort)
					if (a.y > b.y) std::swap(a, b);
					if (a.y > c.y) std::swap(a, c);
					if (b.y > c.y) std::swap(b, c);

					// Total height of the triangle
					int totalHeight = c.y - a.y;

					for (int i = 0; i < totalHeight; i++)
					{
						int ba_y_diff = b.y - a.y;
						int cb_y_diff = c.y - b.y;
						bool secondHalf = i > ba_y_diff || b.y == a.y;
						int segmentHeight = secondHalf ? cb_y_diff : ba_y_diff;
						float alpha = static_cast<float>(i) / totalHeight;
						float beta = static_cast<float>(i - (secondHalf ? ba_y_diff :0)) / segmentHeight;
						Vector2i first = a + (c - a) * alpha;
						Vector2i second = secondHalf ? 
															   b + (c - b) * beta 
															 : a + (b - a) * beta;
						if (first.x > second.x) std::swap(first, second);
						for (int j = first.x; j <= second.x; j++)
						{
							geometry.TrySetPixel(j, a.y + i, color);
						}
					}
				};

				static auto trianglev3 = [&](Vector2i t0, Vector2i t1, Vector2i t2, Color color)
				{
					if (t0.y == t1.y && t0.y == t2.y) return; // i dont care about degenerate triangles
					if (t0.y>t1.y) std::swap(t0, t1);
					if (t0.y>t2.y) std::swap(t0, t2);
					if (t1.y>t2.y) std::swap(t1, t2);
					int total_height = t2.y - t0.y;
					for (int i = 0; i<total_height; i++)
					{
						bool second_half = i>t1.y - t0.y || t1.y == t0.y;
						int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
						float alpha = (float)i / total_height;
						float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here
						Vector2i A = t0 + (t2 - t0)*alpha;
						Vector2i B = second_half ? t1 + (t2 - t1)*beta : t0 + (t1 - t0)*beta;
						if (A.x>B.x) std::swap(A, B);
						for (int j = A.x; j <= B.x; j++)
						{
							geometry.SetPixel(j, t0.y + i, color); // attention, due to int casts t0.y+i != A.y
						}
					}
				};

				trianglev2(t0[0], t0[1], t0[2], sf::Color::Red);
				trianglev2(t1[0], t1[1], t1[2], sf::Color::Green);
				trianglev2(t2[0], t2[1], t2[2], sf::Color::Blue);
				test.AppendImage("Line Sweeping 2", geometry);

			}));
		}
}
}