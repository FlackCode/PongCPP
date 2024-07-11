#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

//float playerPosY = 0.f;
//float playerPosX = 0.f;

//dp = derivative position
float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y, ball_half_size;

internal void simulate_game(Input* input, float dt) {
    clear_screen(0x231C1C);

    draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);

    float player_1_ddp = 0.f;
    if (is_down(BUTTON_W)) player_1_ddp += 2000;
    if (is_down(BUTTON_S)) player_1_ddp -= 2000;

    float player_2_ddp = 0.f;
    if (is_down(BUTTON_UP)) player_2_ddp += 2000;
    if (is_down(BUTTON_DOWN)) player_2_ddp -= 2000;
    //if (is_down(BUTTON_LEFT)) playerPosX -= speed * dt;
    //if (is_down(BUTTON_RIGHT)) playerPosX += speed * dt;

    //adding friction
    player_1_ddp -= player_1_dp * 10.f;

    //equation of motion(basic physics) pos = initpos + velocity * time + (accel * time^2)/2
    player_1_p = player_1_p + player_1_dp * dt + player_1_ddp * dt * dt * .5f;
    //equation of motion(basic physics) velocity = initvelocity + accel * time
    player_1_dp = player_1_dp + player_1_ddp * dt;

    if (player_1_p + player_half_size_y > arena_half_size_y) {
        player_1_p = arena_half_size_y - player_half_size_y;
        player_1_dp = 0;
    }
    else if (player_1_p - player_half_size_y < -arena_half_size_y) {
        player_1_p = -arena_half_size_y + player_half_size_y;
        player_1_dp = 0;
    }

    player_2_ddp -= player_2_dp * 10.f;
    player_2_p = player_2_p + player_2_dp * dt + player_2_ddp * dt * dt * .5f;
    player_2_dp = player_2_dp + player_2_ddp * dt;

    if (player_2_p + player_half_size_y > arena_half_size_y) {
        player_2_p = arena_half_size_y - player_half_size_y;
        player_2_dp = 0;
    }
    else if (player_2_p - player_half_size_y < -arena_half_size_y) {
        player_2_p = -arena_half_size_y + player_half_size_y;
        player_2_dp = 0;
    }

    ball_p_x += ball_dp_x * dt;
    ball_p_y += ball_dp_y * dt;

    //AABB - axis aligned bounding box

    //draw_rect(playerPosX, playerPosY, 10, 10, 0xff0000);

    draw_rect(ball_p_x, ball_p_y, 1, 1, 0xffffff);

    if (ball_p_x + ball_half_size > 80 - player_half_size_x &&
        ball_p_x - ball_half_size < 80 + player_half_size_x &&
        ball_p_y + ball_half_size > player_2_p - player_half_size_y &&
        ball_p_y + ball_half_size < player_2_p + player_half_size_y) {
        ball_p_x = 80 - player_half_size_x - ball_half_size;
        ball_dp_x *= -1;
        //ball_dp_y = player_2_dp * 0.75;
        ball_dp_y = (ball_p_y - player_2_p) * 5;
    } else if (ball_p_x + ball_half_size > -80 - player_half_size_x &&
        ball_p_x - ball_half_size < -80 + player_half_size_x &&
        ball_p_y + ball_half_size > player_1_p - player_half_size_y &&
        ball_p_y + ball_half_size < player_1_p + player_half_size_y) {
        ball_p_x = -80 + player_half_size_x + ball_half_size;
        ball_dp_x *= -1;
        //ball_dp_y = player_1_dp * 0.75;
        ball_dp_y = (ball_p_y - player_1_p) * 5;
    }


    // collisions for the ball with the arena ceiling and floor
    if (ball_p_y + ball_half_size > arena_half_size_y) {
        ball_p_y = arena_half_size_y - ball_half_size;
        ball_dp_y *= -1;
    } else if (ball_p_y - ball_half_size < -arena_half_size_y) {
        ball_p_y = -arena_half_size_y + ball_half_size;
        ball_dp_y *= -1;
    }

    // collision with left and right and resetting ball position on collision
    if (ball_p_x + ball_half_size > arena_half_size_x + ball_half_size) {
        ball_p_x = 0;
        ball_p_y = 0;
        ball_dp_x *= -1;
        ball_dp_y *= -1;
    } else if (ball_p_x - ball_half_size < -arena_half_size_x - ball_half_size) {
        ball_p_x = 0;
        ball_p_y = 0;
        ball_dp_x *= -1;
        ball_dp_y *= -1;
    }

    draw_rect(80, player_2_p, player_half_size_x, player_half_size_y, 0xffffff);
    draw_rect(-80, player_1_p, player_half_size_x, player_half_size_y, 0xffffff);
}