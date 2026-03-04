#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/highmem.h>
#include <linux/string.h>
#include <linux/mm.h>
#include "lut.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Onish Sharma");
MODULE_DESCRIPTION("zIRAM: Intelligent Entropy-Based Memory Triage");

// Formula: H = 12 - (1/4096) * sum(c * log2(c))
static uint32_t calc_entropy(const unsigned char *page) {
  uint32_t counts[256] = {0};
  uint32_t sum_clogc = 0;
  int i;

  // histrogram use loop unrolled
  for (i = 0; i < 4096; i += 8) {
    counts[page[i]]++;   counts[page[i+1]]++;
    counts[page[i+2]]++; counts[page[i+3]]++;
    counts[page[i+4]]++; counts[page[i+5]]++;
    counts[page[i+6]]++; counts[page[i+7]]++;
  }

  // entropy summations
  for (i = 0; i < 256; i++) {
    if (counts[i] > 0) {
      sum_clogc += log_lut[counts[i]];
    }
  }
  // shifted result (scaled by 1024)
  return (12 << 10) - (sum_clogc >> 12);
}

static void ziram_triage_decision(uint32_t entropy) {
  if (entropy < (2 << 10)) {
        printk_ratelimited(KERN_INFO "zIRAM: Low Entropy (%u) -> Target: LZ4\n", entropy);
    } else if (entropy < (7 << 10)) {
        printk_ratelimited(KERN_INFO "zIRAM: Mid Entropy (%u) -> Target: ZSTD\n", entropy);
    } else {
        printk_ratelimited(KERN_INFO "zIRAM: High Entropy (%u) -> BYPASS Initiated\n", entropy);
    }
}

int ziram_write_page(struct page *page) {
    // kmap_atomic creates a temporary window to look at the physical RAM page
    unsigned char *data = kmap_atomic(page);
    uint32_t entropy;

    // entropy of the live page
    entropy = calculate_entropy(data);

    // make the triage decision
    if (entropy > (7 << 10)) {

    }

    kunmap_atomic(data); // close the window
    return 0;
}
