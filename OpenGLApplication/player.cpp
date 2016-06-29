#include "PlayerStates.h"

class Player
{
private:
	PlayerState state = STANDING;
	float moveSpeed = 0.05f;
	float* playerVertexArray;
	int jumpStart = 0;

public:
	float velocity = 3;

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

	void modifyVertical(float moveSpeed)
	{
		playerVertexArray[1] += moveSpeed;
		playerVertexArray[4] += moveSpeed;
		playerVertexArray[7] += moveSpeed;

		if (playerVertexArray[1] < -1 || playerVertexArray[4] < -1 || playerVertexArray[7] < -1)
		{
			playerVertexArray[1] = -1;
			playerVertexArray[4] = -1;
			playerVertexArray[7] = -1;
			state = STANDING;
		}
	}

	void updateJump(int time)
	{
		float calculationTime = (time - jumpStart) / 1000.0f;
		modifyVertical(velocity*calculationTime + (0.5f * -1 * calculationTime * calculationTime));
	}

	void jump(int startTime)
	{
		state = JUMPING;
		jumpStart = startTime;
	}

	float* getVertexArray()
	{
		return playerVertexArray;
	}

	PlayerState getState()
	{
		return state;
	}
};