
Goal: Given a stan file and data, generate a `generated quantities` block, which
will sample from the prior and posterior

example:

```
parameters {
    real mu;
    real sigma;
}

model {
    mu = normal(0, 1);
    sigma = exponential(1);
    y ~ normal(mu, sigma)  # assume y is defined somewhere
}
```

Then the program should return the following:

```
generated quantities {
    real mu_rep;
    real sigma_rep;
    real y_rep;

    mu_rep = normal_rng(0, 1);
    sigma_rep = exponential_rng(1);
    y = normal_rng(mu_rep, sigma_rep);
}
```

Or in cpp(using Stan MKL):

```
double mu_rep, sigma_rep, y_rep;
stan::math::normal_rng<double, double, mt1993> mu_rep(0, 1, rng);
stan::math::exponential_rng<int, mt1993> sigma_rep(1, rng);
stan::math::normal_rng<double, double, mt1993> mu_rep(mu_rep, sigma_rep, rng);
```

## Order of Operations(draft):

1. Parse data
2. Parse trasformed data
3. Create mapping for each data to value and dimension
4. Parse parameters
5. Parse transformed parameters
6. Create mapping for parameters, mapping explicit parameter values if necessary
7. Parse model; map intermediate arguments to distributions/expressions
8. Map remaining parameters to each prior distribution
9. Parse posterior, map intermediate arguments to parameters
10. Generate cpp code and generated quantities


Assumed difficulties:

- user defined functions. we might need to parse them too
- Implicit posteriors(which dont follow `y ~ dist()` or some other form)


## Per-operation procedure

1. Parse program blocks
2. line-by line tokenization through stack + SM parsing
3. In-block variable/parameter map
4. Merge per-block map with global map
5. Generate dependancy tree