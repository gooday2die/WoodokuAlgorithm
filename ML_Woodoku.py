#!/usr/bin/env python
# coding: utf-8

# In[3]:


import numpy as np
import random
import math
import time
import os


# In[4]:


def rotate_90(np_array):
    """
    A function that rotates np array 90 degrees
    """
    return np.rot90(np_array,axes=(-2,-1))


# In[5]:


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


# In[6]:


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


# In[7]:


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


# In[8]:


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


# In[9]:


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


# In[10]:


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


# In[11]:


def clean_up(field):
    """
    A function that cleans up the field if the number is 2
    """
    field[field == -1] = 0
    return field


# In[12]:


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


# In[13]:


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


# In[14]:


def is_game_over(field, to_move):
    """
    A method that determines if the game is over or not
    """
    for i in range(11):
        for j in range(11):
            if can_move_shape(field, to_move, i, j)[0]:
                return False
    return True


# In[15]:


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
            else:
                print("\x1b[31m %d\x1b[0m"% j, end='')
        print("")


# In[16]:


def generate_all_heuristics(field, new_shape, x, y, z, w):
    all_scores = np.zeros((11,11), dtype=int)
    for i in range(11):
        for j in range(11):
            can_move, future_field = can_move_shape(field, new_shape, j, i)
            if can_move:
                all_scores[i][j] = calculate_heuristics(future_field, x, y, z, w)
            else:
                all_scores[i][j] = 0

    return all_scores


# In[103]:


def calculate_heuristics(current_field, x, y, z, w):
    """
    A function that calculates heuristics using ML
    """
    game_score = game_rule(current_field, 0)[1]  # Score if put in this place
    empty_space_score = get_empty_space_score(current_field)
    connection_score = get_connection_score(current_field)
    perfectness_score = get_perfectness(current_field)

    empty_space_score = np.sqrt(np.sqrt(np.sqrt(empty_space_score)))
    connection_score = np.sqrt(np.sqrt(np.sqrt(connection_score)))

    #score = game_score + (math.log10(empty_space_score)) + (math.log10(connection_score))
    #score = game_score * x  + empty_space_score * y + connection_score * z + perfectness_score * w
    #print("GAME SCORE : " + str(game_score))
    #print("Empty Space Score : " + str(empty_space_score))
    #print("Connection Score : " + str(connection_score))
    #print("Perfectness Score : " + str(perfectness_score))
    score = game_score
    return score


# In[104]:


def get_empty_space_score(current_field):
    visited = np.zeros((11,11))
    total_score = 0
    for i in range(11):
        for j in range(11):
            if visited[i][j] == 1 or current_field[i][j] == 1:
                pass
            else:
                score, visited = dfs_empty_space_no_rec(current_field, visited, j, i)
                total_score += score
    return total_score


# In[105]:


def get_connection_score(current_field):
    visited = np.zeros((11,11))
    total_score = 0
    for i in range(11):
        for j in range(11):
            if visited[i][j] == 1 or current_field[i][j] == 0:
                pass
            else:
                score, visited = dfs_connection_no_rec(current_field, visited, j, i)
                total_score += score
    return total_score


# In[106]:


def dfs_connection_no_rec(current_field, visited, x, y):
    stack = [(x,y)]  # make stack
    score = 1  # score gets 2^n
    while (len(stack) != 0):  # if stack not empty
        cur_x, cur_y = stack.pop()  # pop
        if (cur_y > -1 and cur_x > -1) and (cur_y < 11 and cur_x < 11):
            if visited[cur_y][cur_x] == 1:  # if visited, pass
                pass
            else :  # if not out of bound
                if current_field[cur_y][cur_x] == 1:  # and also is filled with pixel
                    visited[cur_y][cur_x] = 1  # set visited
                    score *= 1.5  # score * 2
                    stack.append((cur_x, cur_y-1))  # UP
                    stack.append((cur_x, cur_y+1))  # DN
                    stack.append((cur_x-1, cur_y))  # LEFT
                    stack.append((cur_x+1, cur_y))  # RIGHT
                else:
                    pass
        else:
            pass
    return score, visited # return score


# In[107]:


def dfs_empty_space_no_rec(current_field, visited, x, y):
    stack = [(x,y)]  # make stack
    score = 1  # score gets 2^n
    while (len(stack) != 0):  # if stack not empty
        cur_x, cur_y = stack.pop()  # pop
        if (cur_y > -1 and cur_x > -1) and (cur_y < 11 and cur_x < 11):
            if visited[cur_y][cur_x] == 1:  # if visited, pass
                pass
            else :  # if not out of bound
                if current_field[cur_y][cur_x] == 0:  # and also is not filled with pixel
                    visited[cur_y][cur_x] = 1  # set visited
                    score *= 1.5 # score * 2
                    stack.append((cur_x, cur_y-1))  # UP
                    stack.append((cur_x, cur_y+1))  # DN
                    stack.append((cur_x-1, cur_y))  # LEFT
                    stack.append((cur_x+1, cur_y))  # RIGHT
                else:
                    pass
        else:
            pass
    return score, visited # return score


# In[108]:


def get_perfectness(current_field):
    row_perfectness_list = list()
    col_perfectness_list = list()
    blk_perfectness_list = list()

    # get row_perfectness
    for i in range(1, len(current_field) - 1):
        cur_perfectness = 0
        for j in range(1, len(current_field) - 1):
            cur_perfectness += current_field[i][j]
        row_perfectness_list.append(cur_perfectness)

    # get col_prefectness
    for j in range(1, len(current_field[0]) - 1):
        cur_perfectness = 0
        for i in range(1, len(current_field) - 1):
            cur_perfectness += current_field[i][j]
        col_perfectness_list.append(cur_perfectness)

    # get blk perfectness
    for k in range(3):
        for i in range(3):
            cur_block = list()
            result_list = list()
            cur_perfectness = 0
            for j in range(3):
                cur_block += (current_field[i*3 + 1:i*3 + 3 + 1][j][k*3 + 1:k*3+3 + 1].tolist())
            for i in cur_block:
                cur_perfectness += i
            blk_perfectness_list.append(cur_perfectness)

    return sum(row_perfectness_list) + sum(col_perfectness_list) + sum(blk_perfectness_list)


# In[174]:


def pick_coordinate(heuristics_matrix):
    max_score = np.unique(heuristics_matrix, return_counts=True)[0].max()
    #print(heuristics_matrix)
    possible_matrix = np.zeros((11,11))
    possible_list = list()
    for i in range(11):
        for j in range(11):
            if heuristics_matrix[i][j] == max_score:
                possible_matrix[i][j] = 1
                possible_list.append((i,j))
    #return random.choice(possible_list)
    return possible_list[0]


# In[175]:


def woodoku_algorithm(field, new_shape, x, y, z, w):
    heuristics_matrix = generate_all_heuristics(field, new_shape, x, y, z, w)
    coordinate = pick_coordinate(heuristics_matrix)
    #print("Moving Piece to : " + str(coordinate))
    result, field = move_shape(field, new_shape, coordinate[1], coordinate[0])
    return field


# In[176]:


def print_shape(shape):
    for i in range(len(shape)):
        for j in range(len(shape[0])):
            if shape[i][j] == 0:
                print("\x1b[32m %d\x1b[0m"% shape[i][j], end='')
            else:
                print("\x1b[31m %d\x1b[0m"% shape[i][j], end='')
        print("")


# In[177]:


def count_shape_pixel(shape):
    try:
        return np.unique(shape, return_counts=True)[1][1]
    except IndexError:
        return np.unique(shape, return_counts=True)[1][0]


# In[178]:


field = np.zeros((9,9), dtype=int)
padding_field = np.pad(field, (1,1), 'constant', constant_values=1)
shapes = generate_shapes()
score = 0


# In[179]:


np.save("./shapes.arr", shapes) # save shapes info file for future use with order


# In[180]:


array  = np.load("./shapes.arr.npy", allow_pickle=True).tolist() * 3


# In[181]:


def run(x, y, z, w):
    field = np.zeros((9,9), dtype=int)
    padding_field = np.pad(field, (1,1), 'constant', constant_values=1)
    score = 0
    #np.save("./shapes.arr", shapes) # save shapes info file for future use with order
    #array  = np.load("./shapes.arr.npy", allow_pickle=True).tolist() * 3
    cnt = 1
    try:
        while True:
            #rc = array.pop()
            rc = random.choice(shapes)
            if is_game_over(padding_field, rc):
                break
            print(rc)
            padding_field = woodoku_algorithm(padding_field, rc, x, y, z, w)
            print("SCORE : " + str(score))
            print_screen(padding_field)
            padding_field, new_score = game_rule(padding_field, score)
            score += new_score
            score += count_shape_pixel(rc)
            input("")
            print(cnt)
            cnt += 1
            time.sleep(0.3)
            os.system('clear')
            print("SCORE : %d / TURN : %d"% (score, cnt))

        return score
    except IndexError:
        return 0


# In[ ]:

"""
for i in range(10):
    for j in range(10):
        for k in range(10):
            for l in range(10):
                print("%d %d %d %d / RESULT %d " % (i, j, k, l, run(i,j,k,l)))

"""
# In[153]:


print(run(9, 1, 2, 0))


# In[142]:


# x = 4 is best
# 0 6 1 0 / RESULT 329
# 0 6 6 0 / RESULT 251
# 2 9 2 0 / RESULT 385
# 3 1 1 0 / RESULT 427
# 4 3 2 0 / RESULT 750
# 5 3 2 0 / RESULT 540
# 6 3 2 1 / RESULT 820
# 6 4 3 0 / RESULT 507

