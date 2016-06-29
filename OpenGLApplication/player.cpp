#include "PlayerStates.h"

class Player
{
private:
	PlayerState state = STANDING;
	float moveSpeed = 0.05f;
	float* playerVertexArray;

public:
	Player(int x, int y, int width, int height)
	{
		playerVertexArray = new float[9];
		playerVertexArray[0] = x; playerVertexArray[1] = y; playerVertexArray[2] = 0.0;
		playerVertexArray[3] = x + width; playerVertexArray[4] = y; playerVertexArray[5] = 0.0;
		playerVertexArray[6] = x + (width / 2.0f); playerVertexArray[7] = y + height; playerVertexArray[8] = 0.0;
	}

	void moveRight()
	{
		playerVertexArray[0] += moveSpeed;
		playerVertexArray[3] += moveSpeed;
		playerVertexArray[6] += moveSpeed;
	}

	void moveLeft()
	{
		playerVertexArray[0] -= moveSpeed;
		playerVertexArray[3] -= moveSpeed;
		playerVertexArray[6] -= moveSpeed;
	}

	void jump()
	{
		state = JUMPING;
	}

	float* getVertexArray()
	{
		return playerVertexArray;
	}
};