import tensorflow as tf

matrix_buf = tf.Variable(tf.zeros((2,2)))

matrix1 = tf.constant([[3., 3.]])
matrix2 = tf.constant([[2.], [2.]])
matrix3 = tf.ones((2,2))
matrix4 = tf.random_normal((2,2))

product = tf.matmul(matrix1, matrix2)
add = tf.assign_add(matrix_buf, matrix3)
assign = tf.assign(matrix_buf, matrix4)

print assign

init = tf.initialize_all_variables()

# 4.

with tf.Session() as sess:
    sess.run(init)
    print(product.eval(),"\n")

    for _ in range(3):
        sess.run(add)
        print(matrix_buf.eval())

    sess.run(assign)
    print("\n",matrix_buf.eval(),"\n")