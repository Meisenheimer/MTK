import torch


class MLP(torch.nn.Module):
    def __init__(self):
        super(MLP, self).__init__()
        self.model = torch.nn.Sequential(
            torch.nn.Linear(2, 3, True),
        )

    def forward(self, x):
        return self.model(x)


if __name__ == "__main__":
    mlp = MLP()
    print(mlp.state_dict())
    print(mlp(torch.Tensor([1.0, 0.0])).tolist())
    print(mlp(torch.Tensor([0.0, 1.0])).tolist())
    model = mlp.state_dict()
    fp = open("model", "w")
    for key in model:
        item = model[key].reshape(-1)
        for i in item:
            print(float(i), end=" ", file=fp)
        print("", file=fp)
