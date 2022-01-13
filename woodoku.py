import numpy as np
import random

def rotate_90(np_array):
    """
    A function that rotates np array 90 degrees
    """
    return np.rot90(np_array,axes=(-2,-1))

def generate_rotate_list(np_array):
    """
    A function that gets one array and rotates it three times
    """
    return_list = list()

    return_list.append(np_array)
    return_list.append(rotate_90(np_array))
    return_list.append(rotate_90(rotate_90(np_array)))
    return_list.append(rotate_90(rotate_90(rotate_90(np_array))))

    return return_list

def generate_shapes():
    """
    A function that generates all shape arrays
    There shall be 44 shapes
    """
    shape_list = list()
    t_shape_list = generate_rotate_list(np.array([[1,1,1],[0,1,0]]))
    s_shape_list = generate_rotate_list(np.array([[0,1],[1,1],[1,0]]))
    l_shape_list = generate_rotate_list(np.array([[0,1],[0,1],[1,1]])) + generate_rotate_list(np.array([[1,0],[1,0],[1,1]]))
    r_shape_list = generate_rotate_list(np.array([[1,1],[0,1]]))
    long_T_shape_list = generate_rotate_list(np.array([[1,1,1],[0,1,0]]))
    long_shape_list = [np.array([[1,1]]), np.array([[1],[1]]), np.array([[1,1,1]])
                       ,np.array([[1],[1],[1]]), np.array([[1,1,1,1]]), np.array([[1],[1],[1],[1]])
                      , np.array([[1,1,1,1,1]]), np.array([[1],[1],[1],[1],[1]])]
    big_r_shape_list = generate_rotate_list(np.array([[1,1,1],[0,0,1],[0,0,1]]))
    etc_shape_list = [np.array([1]), np.array([[1,1],[1,1]]), np.array([[0,0,0,1],[0,0,1,0],[0,1,0,0],[1,0,0,0]])
                     ,np.array([[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1]]), np.array([[1,0,0],[0,1,0],[0,0,1]]),
                      np.array([[0,0,1],[0,1,0],[1,0,0]]), np.array([[0,1],[1,0]]), np.array([[1,0],[0,1]])]

    shape_list += t_shape_list
    shape_list += s_shape_list
    shape_list += l_shape_list
    shape_list += r_shape_list
    shape_list += long_T_shape_list
    shape_list += long_shape_list
    shape_list += big_r_shape_list
    shape_list += etc_shape_list

    return shape_list

def game_rule(field, score):
    """
    A function that generates gamerules
    """
    field, row_score = check_rows(field)
    field, col_score = check_cols(field)
    field, block_score = check_block(field)

    if (row_score + col_score + block_score > 2):
        score = 18 * (row_score + col_score + block_score - 1) + 10
    elif (row_score + col_score + block_score == 1):
        score = 18

    print("SCORE : " + str(score))

    field = clean_up(field)

    return field, score

def check_rows(field):
    """
    A function that checks row sets
    If there are any good matched ones, it will switch it to -1
    """
    row_clear = 0
    for i in range(len(field)):
        if field[i].tolist() == [1,1,1,1,1,1,1,1,1]:
            print("Cleared a ROW!")
            row_clear += 1
            field[i] = [-1,-1,-1,-1,-1,-1,-1,-1,-1]

    return field, row_clear

def check_cols(field):
    """
    A function that checks col sets
    If there are any good matched ones, it will switch it to -1
    """
    col_clear = 0
    for i in range(len(field[0])):
        cur_col = list()
        result_list = list()
        for j in range(len(field)):
            cur_col.append(field[j][i])
        for j in cur_col:
            result_list.append(j == -1 or j == 1)
        if result_list == [True, True, True, True, True, True, True, True, True]:
            print("Cleared a COL!")
            col_clear += 1
            for j in range(len(field)):
                field[j][i] = -1

    return field, col_clear

def check_block(field):
    """
    A function that checks block sets
    If there are any good matched ones, it will switch it to -1
    """
    block_clear =0
    for k in range(3):
        for i in range(3):
            cur_block = list()
            result_list = list()
            for j in range(3):
                cur_block += (field[i*3:i*3 + 3][j][k*3:k*3+3].tolist())
            for j in cur_block:
                result_list.append(j == -1 or j == 1)
            if result_list == [True, True, True, True, True, True, True, True, True]:
                for p in range(3):
                    field[i*3:i*3+3][p][k*3:k*3+3] = [-1,-1,-1]
                print("Cleared a Block!")
                block_clear += 1
    return field, block_clear

def clean_up(field):
    """
    A function that cleans up the field if the number is 2
    """
    field[field == -1] = 0
    return field

def move_shape(field, shape, x, y):
    """
    A function that moves a shape into x, y
    """
    org_field = field
    can_move, new_field = can_move_shape(field, shape, x, y)
    print(can_move)
    if (can_move):
        return True, new_field
    else:
        return False, org_field

def can_move_shape(field, to_move, x, y):
    """
    A function that checks if you can move a shape into a x, y
    """

    tmp_field = field.copy()
    if (x < 0  or y < 0):
        return False, field
    else:
        shape_size = to_move.shape
        if (x + shape_size[1] > 9 or y + shape_size[0] > 9):
            return False, field
        else:
            for i in range(shape_size[0]):
                for j in range(shape_size[1]):
                    tmp_field[i + y][j + x] = to_move[i][j] + tmp_field[i + y][j + x]
            if np.any(tmp_field > 1):
                return False, field
            else:
                return True, tmp_field

def is_game_over(field, to_move):
    """
    A method that determines if the game is over or not
    """
    for i in range(9):
        for j in range(9):
            if can_move_shape(field, to_move, i, j):
                return False
    return Truedef is_game_over(field, to_move):
    """
    A method that determines if the game is over or not
    """
    for i in range(9):
        for j in range(9):
            if can_move_shape(field, to_move, i, j):
                return False
    return True

def print_screen(field):
    """
    A function that prints the field to screen
    """
    print("  0 1 2 3 4 5 6 7 8")
    for i in range(len(field)):
        print(str(i), end='')
        for j in field[i]:
            print(" " + str(j), end='')
        print("")


if __name__ == "__main__":
    field = np.zeros((9,9), dtype=int)
    score = 0
    rc = random.choice(all_shapes)

    while not is_game_over(field, rc):
        print("Game Screen : ")
        print_screen(field)

        print("\nNew Block : ")
        print(rc)

        x = int(input("X : "))
        y = int(input("Y : "))
        field = move_shape(field, rc, x, y)[1]
        field, score = game_rule(field, score)
        rc = random.choice(all_shapes)

    print("Game over...")
    print("Score : " + str(score))
