/**
 * Description: bitset that also supports addition, subtraction
 * (twos complement), < operator and setting ranges to 1/0
 */


typedef unsigned long long ull;
const int maxn = 4000*1000+10;
const int num_blocks = (maxn + 63) / 64;
const int num_bits = num_blocks * 64;
constexpr const int mask_64 = 63;
struct bignum
{
	vector<ull> blocks;
	bignum() : blocks(num_blocks) {}

	// set [l,r] to zero
	void zero_range(int l, int r) {
		r = min(r, num_bits - 1);
		int l_block = l / 64;
		int r_block = r / 64;
		repp(i, l_block + 1, r_block) blocks[i] = 0;
		if (l_block == r_block) {
			ull shift = (r + 1) % 64;
			blocks[r_block] &= ~(shift == 0 ? ~0ULL : (1ULL << ((r + 1) % 64)) - 1) | ((1ULL << (l % 64)) - 1);
		}
		else {
			ull shift = (r + 1) % 64;
			blocks[r_block] &= ~(shift == 0 ? ~0ULL : (1ULL << ((r + 1) % 64)) - 1);
			blocks[l_block] &= ((1ULL << (l % 64)) - 1);
		}
	}

	// set [l,r] to one
	void one_range(int l, int r) {
		r = min(r, num_bits - 1);
		int l_block = l / 64;
		int r_block = r / 64;
		repp(i, l_block + 1, r_block) blocks[i] = ~0ULL;
		if (l_block == r_block) {
			ull shift = (r + 1) % 64;
			blocks[r_block] |= (shift == 0 ? ~0ULL : ((1ULL << shift) - 1)) & ~((1ULL << (l % 64)) - 1);
		}
		else {
			ull shift = (r + 1) % 64;
			blocks[r_block] |= (shift == 0 ? ~0ULL : ((1ULL << shift) - 1));
			blocks[l_block] |= ~((1ULL << (l % 64)) - 1);
		}
	}

	bignum operator-(ll p) {
		assert(p >= 0);
		bignum ret = *this;
		if (get(p) == 1) {
			ret.unset(p);
			return ret;
		}
		else {
			int block = p / 64;
			int ind = p % 64;
			int r_ind;
			ull block_mask = ~((1ULL << ind) - 1);
			if (blocks[block] & block_mask)
			{
				r_ind = block * 64 + countr_zero(blocks[block] & block_mask);
			}
			else
			{
				while (block + 1 < sz(blocks) && blocks[++block] == 0) {}
				if (block == sz(blocks)) r_ind = num_blocks * 64;
				else r_ind = block * 64 + countr_zero(blocks[block]);
			}
			ret.one_range(p, r_ind);
			if (r_ind < num_blocks * 64) ret.unset(r_ind);
		}
		return ret;
	}

	bignum operator+(ll p) {
		if (p < 0) return *this - p;
		bignum ret = *this;
		if (get(p) == 0) {
			ret.set(p);
			return ret;
		}
		else {
			int block = p / 64;
			int ind = p % 64;
			int r_ind;
			ull block_mask = (1ULL << ind) - 1;
			if (~(blocks[block] | block_mask))
			{
				r_ind = block * 64 + countr_one(blocks[block] | block_mask);
			}
			else
			{
				while (block + 1 < sz(blocks) && (~blocks[++block]) == 0) {}
				if (block == sz(blocks)) r_ind = num_blocks * 64;
				else r_ind = block * 64 + countr_one(blocks[block]);
			}
			ret.zero_range(p, r_ind);
			if (r_ind < num_blocks * 64) ret.set(r_ind);
		}
		return ret;
	}

	bignum operator<<(ull offset)
	{
		bignum ret = *this;
		for (int block = num_blocks - 1; block >= (int)offset / 64; block--) {
			int high = block - offset / 64;
			int low = high - 1;

			ret.blocks[block] = ret.blocks[high] << (offset & mask_64);
			if (low >= 0 && (offset & mask_64) != 0) {
				ret.blocks[block] |= ret.blocks[low] >> (64 - (offset & mask_64));
			}
		}
		return ret;
	}

	bignum operator|(const bignum& other) {
		bignum ret = *this;
		rep(i, sz(blocks)) ret.blocks[i] |= other.blocks[i];
		return ret;
	}

	int popcount() {
		int ret = 0;
		repe(b, blocks) ret += std::popcount(b);
		return ret;
	}

	void set(int i) {
		int block = i / 64;
		int ind = i % 64;
		blocks[block] |= 1ULL << ind;
	}

	void unset(int i) {
		int block = i / 64;
		int ind = i % 64;
		blocks[block] &= ~(1ULL << ind);
	}

	int get(int i) const {
		int block = i / 64;
		int ind = i % 64;
		return (blocks[block] >> ind) & 1;
	}

	bool operator<(const bignum& other) {
		if (get(num_bits - 1) != other.get(num_bits - 1))
		{
			return !other.get(num_bits - 1);
		}
		for (int i = num_blocks - 1; i >= 0; i--)
		{
			if (blocks[i] == other.blocks[i]) continue;
			return other.blocks[i] > blocks[i];
		}
		return false;
	}

	bool operator==(const bignum& other) {
		return blocks == other.blocks;
	}
};
