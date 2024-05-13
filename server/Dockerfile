FROM rust:1 as builder
WORKDIR /app
COPY . .
RUN cargo install --path .

FROM rust:1 as runner
RUN apt-get update
RUN apt-get install -y --no-install-recommends ca-certificates
RUN update-ca-certificates
COPY --from=builder /usr/local/cargo/bin/smart-bird-cage-server /usr/local/bin/smart-bird-cage-server
COPY --from=builder /app/Rocket.toml Rocket.toml
ENV ROCKET_ADDRESS=0.0.0.0

EXPOSE 80
CMD ["smart-bird-cage-server"]