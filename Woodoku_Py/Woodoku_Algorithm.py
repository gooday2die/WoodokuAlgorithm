#!/usr/bin/env python
# coding: utf-8

# In[379]:


import numpy as np
import random
import math
import timeit


# In[380]:


def rotate_90(np_array):
    """
    A function that rotates np array 90 degrees
    """
    return np.rot90(np_array,axes=(-2,-1))


# In[381]:


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


# In[382]:


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
    etc_shape_list = [np.array([[1]]), np.array([[1,1],[1,1]]), np.array([[0,0,0,1],[0,0,1,0],[0,1,0,0],[1,0,0,0]])
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


# In[383]:


def faster_game_rule(field):
    # check rows
    streak = 0
    row_list = list()
    col_list = list()
    for i in range(1,10):
        if sum(field[i].tolist()) == 11:
            streak += 1
            #print("CLEARED A ROW")
            row_list.append(i)
    # check cols
    for i in range(1,10):
        if sum(field[:][i].tolist()) == 11:
            streak += 1
            #print("CLEARED A COL")
            col_list.append(i)

    for k in range(3):
        for i in range(3):
            cur_block = list()
            result_list = list()
            for j in range(3):
                cur_block += (field[i*3 + 1:i*3 + 3 + 1][j][k*3 + 1:k*3+3 + 1].tolist())
            for j in cur_block:
                result_list.append(j == 1)
            if result_list == [True, True, True, True, True, True, True, True, True]:
                for p in range(3):
                    field[i*3 + 1:i*3+3 + 1][p][k*3 + 1:k*3+3 + 1] = [0,0,0]
                #print("Cleared a Block!")
                streak += 1

    for i in row_list:
        field[i] = [1,0,0,0,0,0,0,0,0,0,1]
    for i in col_list:
        for j in range(1,10):
            field[j][i] = 0

    if streak >= 2:
        return streak * 18 + 10, field
    elif streak == 1:
        return 18, field
    else:
        return 0, field


# In[384]:


def game_rule(field, score):
    """
    A function that generates gamerules
    """
    field, row_score = check_rows(field)
    field, col_score = check_cols(field)
    field, block_score = check_block(field)

    if (row_score + col_score + block_score >= 2):
        score = 18 * (row_score + col_score + block_score - 1) + 10
    elif (row_score + col_score + block_score == 1):
        score = 18
    #print("SCORE : " + str(score))
    else:
        score = 0
    field = clean_up(field)

    return field, score


# In[385]:


def check_rows(field):
    """
    A function that checks row sets
    If there are any good matched ones, it will switch it to -1
    """
    row_clear = 0
    for i in range(1, len(field)-1):
        if field[i].tolist() == [1,1,1,1,1,1,1,1,1,1,1]:
            print("Cleared a ROW!")
            row_clear += 1
            field[i] = [1,-1,-1,-1,-1,-1,-1,-1,-1, -1, 1]

    return field, row_clear


# In[386]:


def check_cols(field):
    """
    A function that checks col sets
    If there are any good matched ones, it will switch it to -1
    """
    col_clear = 0
    for i in range(1, len(field[0]) -1):
        cur_col = list()
        result_list = list()
        for j in range(1, len(field) -1):
            cur_col.append(field[j][i])
        for j in cur_col:
            result_list.append(j == -1 or j == 1)
        if result_list == [True, True, True, True, True, True, True, True, True]:
            print("Cleared a COL!")
            col_clear += 1
            for j in range(1, len(field)-1):
                field[j][i] = -1

    return field, col_clear


# In[387]:


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
                cur_block += (field[i*3 + 1:i*3 + 3 + 1][j][k*3 + 1:k*3+3 + 1].tolist())
            for j in cur_block:
                result_list.append(j == -1 or j == 1)
            if result_list == [True, True, True, True, True, True, True, True, True]:
                for p in range(3):
                    field[i*3 + 1:i*3+3 + 1][p][k*3 + 1:k*3+3 + 1] = [-1,-1,-1]
                print("Cleared a Block!")
                block_clear += 1
    return field, block_clear


# In[388]:


def clean_up(field):
    """
    A function that cleans up the field if the number is 2
    """
    field[field == -1] = 0
    return field


# In[389]:


def move_shape(field, shape, x, y):
    """
    A function that moves a shape into x, y
    """
    org_field = field
    can_move, new_field = can_move_shape(field, shape, x, y)
    #print("SHAPE SIZE : " + str(shape.shape))
    if (can_move):
        return True, new_field
    else:
        return False, org_field


# In[390]:


def can_move_shape(field, to_move, x, y):
    """
    A function that checks if you can move a shape into a x, y
    """
    shape_size = (len(to_move[0]), len(to_move))
    tmp_field = field.copy()
    if (x < 1  or y < 1):
        return False, field
    else:
        #shape_size = to_move.shape
        if (x + shape_size[0] > 10 or y + shape_size[1] > 10):
            return False, field
        else:
            for i in range(shape_size[1]):
                for j in range(shape_size[0]):
                    tmp_field[i + y][j + x] = to_move[i][j] + tmp_field[i + y][j + x]
            if np.any(tmp_field > 1):
                return False, field
            else:
                return True, tmp_field


# In[391]:


def is_game_over(field, to_move):
    """
    A method that determines if the game is over or not
    """
    for i in range(11):
        for j in range(11):
            if can_move_shape(field, to_move, i, j)[0]:
                return False
    return True


# In[605]:


def print_screen(field):
    """
    A function that prints the field to screen
    """
    print("   0 1 2 3 4 5 6 7 8 9 10")
    for i in range(len(field)):
        print("%02d" % i, end='')
        for j in field[i]:
            if j == 0:
                print("\x1b[32m %d\x1b[0m"% j, end='')
            elif j == 1:
                print("\x1b[31m %d\x1b[0m"% j, end='')
            elif j == -1:
                print("\x1b[33m %d\x1b[0m"% j, end='')

        print("")


# In[393]:


def generate_all_futures(field, shape_list):
    future_list = list()

    for i in range(1, 10):
        for j in range(1, 10):
            for k in range(1, 10):
                for l in range(1, 10):
                    for m in range(1, 10):
                        for n in range(1, 10):
                            result1 = can_move_shape(field, shape_list[0],j, i)
                            if result1[0]:
                                result2 = can_move_shape(result1[1], shape_list[1], l, k)
                                if result2[0]:
                                    result3 = can_move_shape(result2[1], shape_list[2], n, m)
                                    if result3[0]:
                                        future_list.append((result3[1], i, j, k, l, m, n))

    return future_list




# In[394]:


def generate_every_future_heuristics(all_futures):
    heuristics_list = list()
    for i in all_futures:
        heuristics_list.append(faster_game_rule(i[0])[0])
    return heuristics_list


# In[399]:


def convert5_5(all_shapes):
    return_list = list()
    for i in all_shapes:
        zeros = np.zeros((5,5))
        cur_shape = i
        for i in range(5):
            for j in range(5):
                try:
                    zeros[i][j] = cur_shape[i][j]
                except IndexError:
                    zeros[i][j] = 0
        return_list.append(zeros)
    return return_list


# In[429]:


def print_blocks(blocks):
    three_blocks = convert5_5(blocks)
    for shape in three_blocks:
        for i in range(5):
            for j in range(5):
                if shape[i][j] == 0:
                    print("\x1b[32m %d\x1b[0m"% shape[i][j], end='')
                else:
                    print("\x1b[31m %d\x1b[0m"% shape[i][j], end='')
            print("")
        print("============")


# In[656]:


def visualize(future, shape_list):
    field = future[0].copy()
    coord_list = list()

    coord_list.append(future[1:3])
    coord_list.append(future[3:5])
    coord_list.append(future[5:7])

    for i in range(len(shape_list)):
        print(str(i) + " SHAPE : to " + str(coord_list[i]))
        for j in range(len(shape_list[i])):
            for k in range(len(shape_list[i][j])):
                field[coord_list[i][0] + j][coord_list[i][1] + k] = shape_list[i][j][k] * (i + 2)

    print("   0 1 2 3 4 5 6 7 8 9 10")
    for i in range(len(field)):
        print("%02d" % i, end='')
        for j in field[i]:
            if j == 0:
                print("\x1b[32m %d\x1b[0m"% j, end='')
            elif j == 1:
                print("\x1b[31m %d\x1b[0m"% j, end='')
            elif j == 2:
                print("\x1b[33m %d\x1b[0m"% j, end='')
            elif j == 3:
                print("\x1b[34m %d\x1b[0m"% j, end='')
            elif j == 4:
                print("\x1b[36m %d\x1b[0m"% j, end='')

        print("")
def count_shape_pixel(shape):
    try:
        return np.unique(shape, return_counts=True)[1][1]
    except IndexError:
        return np.unique(shape, return_counts=True)[1][0]


# In[658]:


def runAlgorithm():
    field = np.zeros((9,9), dtype=int)
    padding_field = np.pad(field, (1,1), 'constant', constant_values=1)
    all_shapes = generate_shapes()
    score = 0
    cnt = 1
    while True:
        shape_list = list()
        for i in range(3):
            shape_list.append(random.choice(all_shapes))
        print_blocks(shape_list)
        start = timeit.default_timer()
        all_futures = generate_all_futures(padding_field, shape_list)
        if len(all_futures) == 0:
                break
        else:
            heuristics_list = generate_every_future_heuristics(all_futures)
            end = timeit.default_timer()

            max_index = heuristics_list.index(max(heuristics_list))
            padding_field = all_futures[max_index][0]

            score += heuristics_list[max_index]
            score += count_shape_pixel(shape_list[0])
            score += count_shape_pixel(shape_list[1])
            score += count_shape_pixel(shape_list[2])
            print_screen(padding_field)
            visualize(all_futures[max_index], shape_list)
            print("SCORE : " + str(score))
            print("TURNS : " + str(cnt))
            print("TIME SPENT : " + str(end - start))
            print("MOVING SHAPE 1 : " + str(all_futures[max_index][1:3]))
            print("MOVING SHAPE 2 : " + str(all_futures[max_index][3:5]))
            print("MOVING SHAPE 3 : " + str(all_futures[max_index][5:7]))
            input("")

    print("GAME OVER")
    print("TOTAL SCORE : " + str(score))

    return score


# In[ ]:
runAlgorithm()



