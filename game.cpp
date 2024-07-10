#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

//float playerPosY = 0.f;
//float playerPosX = 0.f;

//dp = derivative position
float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85;
float arena_half_size_y = 45;
float player_half_size_x = 2.5;
float player_half_size_y = 12;

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

    //draw_rect(playerPosX, playerPosY, 10, 10, 0xff0000);

    draw_rect(0, 0, 1, 1, 0xffffff);

    draw_rect(80, player_2_p, player_half_size_x, player_half_size_y, 0xffffff);
    draw_rect(-80, player_1_p, 2.5, 12, 0xffffff);
}