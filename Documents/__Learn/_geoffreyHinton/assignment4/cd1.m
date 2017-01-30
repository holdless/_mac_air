function ret = cd1(rbm_w, visible_data)
% <rbm_w> is a matrix of size <number of hidden units> by <number of visible units>
% <visible_data> is a (possibly but not necessarily binary) matrix of size <number of visible units> by <number of data cases>
% The returned value is the gradient approximation produced by CD-1. It's of the same shape as <rbm_w>.

h0_prob = logistic(rbm_w*visible_data);
h0 = sample_bernoulli(h0_prob);
gradient0 = h0*visible_data';
v1_prob = logistic(rbm_w'*h0);
v1 = sample_bernoulli(v1_prob);
h1_prob = logistic(rbm_w*v1);
% h1 = sample_bernoulli(h1_prob);
% gradient1 = h1*v1';
% Q7: replace h1 by h1_prob for variance reduction
gradient1 = h1_prob*v1';

ret = (gradient0 - gradient1)/size(visible_data, 2);

end
