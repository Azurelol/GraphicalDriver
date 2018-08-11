#pragma once

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Vector.h"

namespace Oxana
{
	using Color = sf::Color;

	// Class for basic image manipulation
	class Image
	{
		sf::Image image;
		Vector2f size;
		Color fillColor;

		public:
		Image(float width, float height, Color color = sf::Color::White)
		{
			this->Reset(width, height, color);
		}

		void Save(const std::string& path)
		{
			image.saveToFile(path.c_str());
		}

		void Reset(float width, float height, Color fillColor = sf::Color::White)
		{
			this->size = Vector2f(width, height);
			this->fillColor = fillColor;
			this->Reset();
		}

		void Reset()
		{					
			image.create(this->size.x, this->size.y, this->fillColor);
		}

		void SetPixel(int x, int y, Color color)
		{			
			image.setPixel(x, y, color);
		}

		bool TrySetPixel(int x, int y, Color color)
		{
			if (!ValidatePixel(x, y))
				return false;

			image.setPixel(x, y, color);
			return true;
		}

		bool ValidatePixel(int x, int y)
		{
			return !(x >= size.x || y >= size.y);				
		}

		void FlipVertically()
		{
			image.flipVertically();
		}

		void FlipHorizontally()
		{
			image.flipHorizontally();
		}

		sf::Texture ToTexture() const
		{
			sf::Texture texture;
			texture.loadFromImage(this->image);
			return texture;
		}

		//static float Lerp(float a, float b, float t)
		//{
		//	return a
		//}

	};

	// Class used for loading an iamge into a texture to be drawn
	struct Texture
	{
		std::string name;
		std::string path;
		sf::Texture texture;
		bool loaded;

		Texture(const sf::Image& image)
		{			
			this->loaded = this->texture.loadFromImage(image);
		}

		Texture(const sf::Texture& texture)
		{
			this->texture = texture;
			this->loaded = true;
		}

		Texture(const std::string& name, const std::string& path, bool load = true) : name(name), path(path)
		{	
			if (load)
				this->Load();
		}

		void Load()
		{
			this->loaded = this->texture.loadFromFile(this->path);
		}		
	};

}