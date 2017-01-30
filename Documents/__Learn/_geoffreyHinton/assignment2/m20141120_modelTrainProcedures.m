

load data.mat;
[train_x, train_t, valid_x, valid_t, test_x, test_t, vocab] = load_data(100);
model = train(1);