.PHONY: all build build-prod run clean test

APP_NAME = xapi-cli
VERSION  = v0.1.2
DATE     = $(shell date +%Y-%m-%d\ %H:%M)
BUILDDATE = $(shell date +%Y%m%d%H%M)

all: build-prod

build: clean
	@echo "Building $(APP_NAME)..."
	go build -ldflags "-X 'main.Version=$(VERSION)' -X 'main.BuildDate=$(DATE)'" -o $(APP_NAME) .
	upx --best --lzma $(APP_NAME)

build-prod: clean
	@echo "Building $(APP_NAME) $(VERSION) for production..."
	CGO_ENABLED=0 go build -ldflags "-s -w -X 'main.Version=$(VERSION)' -X 'main.BuildDate=$(DATE)'" -o $(APP_NAME) .
	upx --best --lzma $(APP_NAME)

run: build
	@echo "Running $(APP_NAME)..."
	@./$(APP_NAME)

clean:
	@echo "Cleaning up..."
	@rm -rf $(APP_NAME)

test:
	@echo "Running tests..."
	@go test ./...

tidy:
	@echo "Tidying go modules..."
	go mod tidy

deps:
	@echo "Installing dependencies..."
	go mod download