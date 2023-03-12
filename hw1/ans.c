#include <iostream>
#include <vector>

void acceleration() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

std::vector<std::vector<int64_t>> matrix;
std::vector<std::vector<int64_t>> ans;

int main() {
    acceleration();
    int n, m, q;
    std::cin >> n >> m >> q;
    matrix.resize(n, std::vector<int64_t>(m));
    ans.resize(n + 1, std::vector<int64_t>(m + 1));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> matrix[i][j];
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            ans[i][j] = ans[i - 1][j] + ans[i][j - 1] - ans[i - 1][j - 1] + matrix[i - 1][j - 1];
        }
    }

    for (int i = 0; i < q; i++) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        std::cout << ans[x2][y2] - ans[x1 - 1][y2] - ans[x2][y1 - 1] + ans[x1 - 1][y1 - 1] << '\n';
    }
    return 0;
}
