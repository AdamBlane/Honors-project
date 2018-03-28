#pragma once
//for A*
class node
{
	private:
		int x_coord, y_coord, distance, weight;

	public:
		node(int x, int z, int d, int p)
		{
			x_coord = x; y_coord = z; distance = d; weight = p;
		}

		int getxPos() const { return x_coord; }
		int getyPos() const { return y_coord; }
		int getDistance() const { return distance; }
		int getWeight() const { return weight; }

		void UpdatePriority(const int & xDestination, const int & zDestination)
		{
			weight = distance + Estimate(xDestination, zDestination) * 10; //A*
		}

		// give better priority to going straight instead of diagonally
		void NextDistance(const int & i) // i: direction
		{
			distance += (i % 2 == 0 ? 10 : 14);
		}

		// Estimation function for the remaining distance to the goal.
		const int & Estimate(const int & xDest, const int & yDest) const
		{
			static int xd, yd, d;
			xd = xDest - x_coord;
			yd = yDest - y_coord;

			// Euclidian
			d = static_cast<int>(sqrt(xd*xd + yd * yd));

			return(d);
		}
};