kernel void update_positions(const float delta_time,
                             global float* x_positions, global float* y_positions,
                             global const float* x_velocities, global const float* y_velocities)
{
	size_t id = get_global_id(0);

	x_positions[id] += x_velocities[id] * delta_time;
	y_positions[id] += y_velocities[id] * delta_time;
}

kernel void update_velocities(const float delta_time,
                              global const float* x_positions, global const float* y_positions,
                              global float* x_velocities, global float* y_velocities,
                              global const float* masses)
{
	size_t id = get_global_id(0);
	float2 acceleration = (float2)0.0f;
	float2 error = (float2)0.0f;

	for (size_t i = 0; i < get_global_size(0); i++)
	{
		if (i == id) continue;

		float2 difference = (float2)(x_positions[i] - x_positions[id], y_positions[i] - y_positions[id]);

		float2 direction = fast_normalize(difference);
		float distance2 = dot(difference, difference);
		float2 result = masses[i] / distance2 * direction;

		float2 delta = result - error;
		float2 total = acceleration + delta;
		error = (total - acceleration) - delta;
		acceleration = total;
	}

	const float G = 6.67430E-13f;
	acceleration *= G * delta_time;

	x_velocities[id] += acceleration.x;
	y_velocities[id] += acceleration.y;
}