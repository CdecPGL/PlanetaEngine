# メモリアロケータについて

[http://marupeke296.com/ALG_No2_TLSFMemoryAllocator.html](http://marupeke296.com/ALG_No2_TLSFMemoryAllocator.html)を参考にTLSFを実装。

実装したが、今のところ使う予定はない。

## MSBアルゴリズム

選択肢は複数ある。

### [http://marupeke296.com/cgi-bin/cbbs/cbbs.cgi?mode=al2&namber=5198&rev=&no=0&P=R&KLOG=5](ここ)の方法

```:cpp
static unsigned count1_32(unsigned val) {
	val = (val & 0x55555555) + ((val >> 1) & 0x55555555);
	val = (val & 0x33333333) + ((val >> 2) & 0x33333333);
	val = (val & 0x0f0f0f0f) + ((val >> 4) & 0x0f0f0f0f);
	val = (val & 0x00ff00ff) + ((val >> 8) & 0x00ff00ff);
	return  (val & 0x0000ffff) + ((val >> 16) & 0x0000ffff);
}
static size_t MSB32(unsigned val) {
	if (val == 0) return 0;
	val |= (val >> 1);
	val |= (val >> 2);
	val |= (val >> 4);
	val |= (val >> 8);
	val |= (val >> 16);
	return count1_32(val) - 1;
}
```

### 論外な方法

```:cpp
ret = (size_t)std::log2l(num) + 1;
```

### 入力が大きいと時間がかかる方法

```:cpp
ret = 0;
size_t buf = num;
while ((buf >>= 1) >= 1){ ++ret; }
++ret;
```

## 一つ目と互角な方法

最良条件ではこっちの方が、最悪条件では1の方が早い。また、入力値が確定する状況下では最適化により1はコンパイル時計算され、コストはゼロになる。

```:cpp
const size_t e2_map[32] = {
	1u << 0, 1u << 1, 1u << 2, 1u << 3, 1u << 4, 1u << 5, 1u << 6, 1u << 7, 1u << 8, 1u << 9,
	1u << 10, 1u << 11, 1u << 12, 1u << 13, 1u << 14, 1u << 15, 1u << 16, 1u << 17, 1u << 18, 1u << 19,
	1u << 20, 1u << 21, 1u << 22, 1u << 23, 1u << 24, 1u << 25, 1u << 26, 1u << 27, 1u << 28, 1u << 29,
	1u << 30, 1u << 31
};
ret = 16;
unsigned int seg = 16;
while ((seg >>= 1)){
	/*ret += (1 - (int)(num < e2_map[ret - 1]) * 2)*seg;*/
	size_t e2 = e2_map[ret - 1];
	if (num > e2){ ret += seg; }
	else if (num < e2){ ret -= seg; }
	else{ break; }
}			
if (e2_map[ret - 1] > num){ --ret; }
```

### 結論

最後のを使うことにした。(LSBは一つ目の方法のサイトのものを使用)
