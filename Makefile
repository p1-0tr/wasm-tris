HEADERS := $(shell find src -name *.hpp)
SRC := $(shell find src -name *.cpp)
OBJS := $(SRC:src/%.cpp=out/%.o)
ASSETS_DIR := assets/
ASSETS := $(shell find $(ASSETS_DIR) -name *.png -o -name *.vert -o -name *.frag)
PREBUILTS := $(shell find vendor/soil -name *.bc)
JS_SRC := $(shell find page -name *.js)
JS_OBJS := $(JS_SRC:page/%.js=out/%.js)
CSS_SRC := $(shell find page -name *.css)
CSS_OBJS := $(CSS_SRC:page/%.css=out/%.css)

OUT_DIR = out

INCLUDES = -I vendor/soil
EMCC_FLAGS = -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=1 -s EXPORTED_FUNCTIONS="['_user_input', '_main', '_next_frame']"
CPP_FLAGS = -std=c++11

WASM_HTML = $(OUT_DIR)/wasm-tris.html
HTML_TPL = page/wasm-tris.html.tpl

.PHONY: clean all
all: $(WASM_HTML) $(JS_OBJS) $(CSS_OBJS)

$(WASM_HTML): $(OBJS) $(HTML_TPL) $(ASSETS)
	emcc -o $(WASM_HTML) $(OBJS) $(PREBUILTS) $(INCLUDES) $(EMCC_FLAGS) $(CPP_FLAGS) --shell-file $(HTML_TPL) --preload-file $(ASSETS_DIR)

$(OUT_DIR):
	mkdir out

$(OUT_DIR)/%.o: src/%.cpp $(HEADERS) | $(OUT_DIR)
	emcc -o $@ $< $(CPP_FLAGS) $(INCLUDES)

$(OUT_DIR)/%.css: page/%.css | $(OUT_DIR)
	cp $< $@

$(OUT_DIR)/%.js: page/%.js | $(OUT_DIR)
	cp $< $@

clean:
	rm -r $(OUT_DIR)
