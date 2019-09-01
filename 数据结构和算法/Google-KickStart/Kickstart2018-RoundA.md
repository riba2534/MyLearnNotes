# Kickstart2018-Round-A

## Even Digits(思路)

### Problem

> Supervin has a unique calculator. This calculator only has a display, a plus button, and a minus button. Currently, the integer **N** is displayed on the calculator display.
>
> Pressing the plus button increases the current number displayed on the calculator display by 1. Similarly, pressing the minus button decreases the current number displayed on the calculator display by 1. The calculator does not display any leading zeros. For example, if `100` is displayed on the calculator display, pressing the minus button once will cause the calculator to display `99`.
>
> Supervin does not like odd digits, because he thinks they are "odd". Therefore, he wants to display an integer with only even digits in its decimal representation, using only the calculator buttons. Since the calculator is a bit old and the buttons are hard to press, he wants to use a minimal number of button presses.
>
> Please help Supervin to determine the minimum number of button presses to make the calculator display an integer with no odd digits.

### Input

> The first line of the input gives the number of test cases, **T**. **T** test cases follow. Each begins with one line containing an integer **N**: the integer initially displayed on Supervin's calculator.

### Output

> For each test case, output one line containing `Case #x: y`, where `x` is the test case number (starting from 1) and `y` is the minimum number of button presses, as described above.

### Limits

> 1 ≤ **T** ≤ 100.
> Time limit: 20 seconds per test set.
> Memory limit: 1GB.

#### Small dataset (Test set 1 - Visible)

> 1 ≤ **N** ≤ 10^5.

#### Large dataset (Test set 2 - Hidden)

> 1 ≤ **N** ≤ 10^16.

### Sample

| Input                | Output                                            |
| -------------------- | ------------------------------------------------- |
| `4 42 11 1 2018    ` | `Case #1: 0 Case #2: 3 Case #3: 1 Case #4: 2    ` |

> In Sample Case #1, the integer initially displayed on the calculator has no odd digits, so no button presses are needed.
>
> In Sample Case #2, pressing the minus button three times will cause the calculator to display `8`. There is no way to satisfy the requirements with fewer than three button presses.
>
> In Sample Case #3, either pressing the minus button once (causing the calculator to display `0`) or pressing the plus button once will cause the calculator to display an integer without an odd digit.
>
> In Sample Case #4, pressing the plus button twice will cause the calculator to display `2020`. There is no way to satisfy the requirements with fewer than two button presses.

### 思路

先说题意，给你一个数，要把它变成一个 `各个位数全部是偶数的数字` ，你有两个按钮，一个按钮是 `+` 按钮，按一下数字增加1，还有一个按钮是 `- ` 按钮，按一下数字减少 1 ，求你至少按下按钮多少次，可以把这个数字变成每个位都是偶数的数字。

首先，数据范围很大，不能暴力。我们由题意可以确定，要么找一个比它大的全部是偶数的数字，要么找一个比他小的全部是偶数的数字，所以我们的思路就是，构造这两个数字，然后和给出的数字求绝对值，去最小的即可。

那么如何构造比它大的数字呢，对于一个数字，从左到右，如果已经是偶数，那么不用管，看下一个数字，我们要做的是把某一位数字变成比它大的偶数，后面的数字不用管，直接可以填0，比如 `1234 -> 2000` ，`6889 -> 8000` 。在构造比它小的数字时，我们要尽量的用 8 来填，比如 `1111 -> 888` ，`2111 -> 2088` ，从左到右，如果是偶数不用管，如果是奇数，那么直接填上比它小的偶数，后面用8来补。

### 代码

```cpp
#include <bits/stdc++.h>
using namespace std;
#define mem(a, b) memset(a, b, sizeof(a))
typedef long long ll;
const ll N = 100 + 10;
ll num1[N], num2[N], vis[N], cnt;
ll is_even(ll n)
{
    while (n)
    {
        ll x = n % 10;
        if (x & 1)
            return 0;
        n /= 10;
    }
    return 1;
}
ll abs(ll x, ll y)
{
    if (x > y)
        return x - y;
    return y - x;
}
ll get_next_even(ll x)
{
    if (x & 1)
        return x + 1;
    return x + 2;
}
ll get_pre_even(ll x)
{
    if (x & 1)
        return x - 1;
    return x - 2;
}

ll get_next()
{
    memcpy(num2, num1, sizeof(num1));
    ll n = cnt;
    mem(vis, 0);
    for (ll i = n - 1; i >= 0; i--)
    {
        if (get_next_even(num2[i]) >= 10)
            vis[i] = 1;
        else
            vis[i] = 0;
    }
    for (ll i = n - 1; i >= 0; i--)
    {
        if (num2[i] & 1)
        {
            if (!vis[i])
            {
                num2[i] = get_next_even(num2[i]);
                for (ll j = i - 1; j >= 0; j--)
                    num2[j] = 0;
                break;
            }
            else
            {
                ll j;
                for (j = i; j < n; j++)
                    if (!vis[j])
                        break;
                if (j == n)
                    n++;
                num2[j] = get_next_even(num2[j]);
                for (j = j - 1; j >= 0; j--)
                    num2[j] = 0;
                break;
            }
        }
    }
    ll ans = 0;
    for (ll i = n - 1; i >= 0; i--)
        ans = ans * 10 + num2[i];
    return ans;
}

ll get_pre()
{
    memcpy(num2, num1, sizeof(num1));
    ll n = cnt;
    for (ll i = n - 1; i >= 0; i--)
    {
        if (num2[i] & 1)
        {
            num2[i] = get_pre_even(num2[i]);
            for (ll j = i - 1; j >= 0; j--)
                num2[j] = 8;
            break;
        }
    }
    ll ans = 0;
    for (ll i = n - 1; i >= 0; i--)
        ans = ans * 10 + num2[i];
    return ans;
}
void solve(ll n)
{
    if (is_even(n))
    {
        puts("0");
        return;
    }
    cnt = 0;
    ll x = n;
    while (x)
    {
        num1[cnt++] = x % 10;
        x /= 10;
    }
    printf("%lld\n", min(abs(get_next(), n), abs(get_pre(), n)));
}
int main()
{
    // freopen("in.txt", "r", stdin);
    ll t, n, q = 1;
    scanf("%lld", &t);
    while (t--)
    {
        scanf("%lld", &n);
        printf("Case #%lld: ", q++);
        solve(n);
    }
    return 0;
}
```

